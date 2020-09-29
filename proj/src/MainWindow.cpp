#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QTreeView>
#include <QSplitter>
#include <QSettings>
#include <QProcess>
#include <QScrollBar>
#include <QInputDialog>
#include <QMessageBox>
#include <QClipboard>

#include "AboutDialog.h"
#include "TreeModel.h"
#include "TreeItem.h"
#include "RenameDialog.h"
#include "NewTagDialog.h"
#include "FindDialog.h"
#include "FindChunkDialog.h"
#include "RecentFilesMenu.h"

#define TITLE "Minecraft Explorer"

#define SETTINGS_APPLICATION "MinecraftExplorer"
#define SETTINGS_ORGANIZATION "Germix"

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    lastFindItem = nullptr;
    lastFindPosition = -1;

    viewWidget = new QWidget();

    treeModel = new TreeModel();
    treeModelView = new QTreeView();
    treeModelView->setModel(treeModel);
    treeModelView->setHeaderHidden(true);
    treeModelView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeModelView,
            SIGNAL(customContextMenuRequested(QPoint)),
            this,
            SLOT(slotTreeView_customContextMenuRequested(QPoint)));
    connect(treeModel, SIGNAL(onModified()), this, SLOT(slotModelModified()));
    connect(treeModelView->selectionModel(),
        SIGNAL(currentChanged(QModelIndex,QModelIndex)),
        this,
        SLOT(slotTreeView_currentChanged(QModelIndex,QModelIndex)));
    connect(treeModelView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotTreeView_doubleClicked(QModelIndex)));

    splitter = new QSplitter();
    splitter->addWidget(treeModelView);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);
    splitter->setOrientation(Qt::Horizontal);
    setCentralWidget(splitter);

    ui->menu_View->addAction(ui->mainToolBar->toggleViewAction());

    //
    // Actions
    //
    actionDirUp = new QAction(QIcon(":/images/nav-up.png"), tr("Move up"));
    actionDirEnter = new QAction(QIcon(":/images/nav-enter.png"), tr("Enter directory"));
    actionOpenContainerFolder = new QAction(QIcon(":/images/file-open-container-folder.png"), tr("Open container folder"));

    //
    // Clipboard
    //
    const QClipboard* cb = QApplication::clipboard();
    connect(cb, SIGNAL(dataChanged()), this, SLOT(slotClipboard_dataChanged()));

    //
    // Load settings
    //
    QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);

    restoreGeometry(s.value("WindowGeometry").toByteArray());
    restoreState(s.value("WindowState").toByteArray());
    splitter->restoreGeometry(s.value("SplitterGeometry").toByteArray());
    splitter->restoreState(s.value("SplitterState").toByteArray());

    initRecentFilesMenu(s.value("RecentFiles").toByteArray());

    openFolder(s.value("CurrentFolder", "").toString());
}

MainWindow::~MainWindow()
{
    QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);
    s.setValue("WindowState", saveState());
    s.setValue("WindowGeometry", saveGeometry());
    s.setValue("SplitterState", splitter->saveState());
    s.setValue("SplitterGeometry", splitter->saveGeometry());
    s.setValue("RecentFiles", recentFiles->saveState());
    s.setValue("CurrentFolder", currentFolder);

    delete ui;
}

void MainWindow::updateActions()
{
    ui->actionFileSave->setEnabled(treeModel->isModified());

    QModelIndex index = treeModelView->currentIndex();
    TreeItem* item = treeModel->toItem(index);
    if(item != nullptr)
    {
        ui->actionFind->setEnabled(item->canFind());
        ui->actionFindNext->setEnabled(item == lastFindItem);
        ui->actionFindChunk->setEnabled(item->canFindChunk());

        ui->actionCut->setEnabled(item->canCutItem());
        ui->actionCopy->setEnabled(item->canCopyItem());
        ui->actionPaste->setEnabled(item->canPasteIntoItem());

        ui->actionEdit->setEnabled(item->canEdit());
        ui->actionDelete->setEnabled(item->canDelete());
        ui->actionRename->setEnabled(item->canRename());
        ui->actionRefresh->setEnabled(item->canRefresh());
        ui->actionMoveItemUp->setEnabled(item->canMoveUp());
        ui->actionMoveItemDown->setEnabled(item->canMoveDown());

        checkNbtTag(item, ui->actionAddByteTag, NBTTAG_BYTE);
        checkNbtTag(item, ui->actionAddShortTag, NBTTAG_SHORT);
        checkNbtTag(item, ui->actionAddIntTag, NBTTAG_INT);
        checkNbtTag(item, ui->actionAddLongTag, NBTTAG_LONG);
        checkNbtTag(item, ui->actionAddFloatTag, NBTTAG_FLOAT);
        checkNbtTag(item, ui->actionAddDoubleTag, NBTTAG_DOUBLE);
        checkNbtTag(item, ui->actionAddByteArrayTag, NBTTAG_BYTE_ARRAY);
        checkNbtTag(item, ui->actionAddStringTag, NBTTAG_STRING);
        checkNbtTag(item, ui->actionAddListTag, NBTTAG_LIST);
        checkNbtTag(item, ui->actionAddCompoundTag, NBTTAG_COMPOUND);
        checkNbtTag(item, ui->actionAddIntArrayTag, NBTTAG_INT_ARRAY);
        checkNbtTag(item, ui->actionAddLongArrayTag, NBTTAG_LONG_ARRAY);
    }
}

void MainWindow::addNbtTag(int type)
{
    QModelIndex index = treeModelView->currentIndex();

    if(index.isValid())
    {
        TreeItem* item = treeModel->toItem(index);

        if(dynamic_cast<TreeItemNbtTagList*>(item) != nullptr)
        {
            treeModel->addNbtTag(index, type, QString());
        }
        else
        {
            NewTagDialog(treeModel, type, index).exec();
        }
    }
}

void MainWindow::checkNbtTag(TreeItem* parent, QAction* action, int type)
{
    action->setVisible(parent->canAddNbtTag(type));
    action->setEnabled(parent->canAddNbtTag(type));
}

void MainWindow::findNextItem()
{
    if(lastFindIndex.isValid())
    {
        QModelIndex indexFound = treeModel->findItem(lastFindIndex, lastFindPosition+1, lastFindName, lastFindValue);
        if(indexFound.isValid())
        {
            lastFindItem = treeModel->toItem(indexFound);
            lastFindIndex = indexFound.parent();
            lastFindPosition = indexFound.row();
            treeModelView->setCurrentIndex(indexFound);
        }
        else
        {
            lastFindItem = nullptr;
            lastFindIndex = QModelIndex();
            lastFindPosition = -1;
            lastFindName = QString();
            lastFindValue = QString();
            updateActions();
            QMessageBox::information(this, tr("Find"), tr("Item not found"));
        }
    }
}

void MainWindow::openFolder(const QString& folder)
{
    recentFiles->addFile(QFileInfo(currentFolder).absoluteFilePath());
    recentFiles->removeFile(QFileInfo(folder).absoluteFilePath());
    currentFolder = folder;

    if(!currentFolder.isEmpty())
    {
        treeModel->load(currentFolder);
    }
    lastFindItem = nullptr;
    lastFindPosition = -1;

    updateActions();
}

void MainWindow::initRecentFilesMenu(const QByteArray& state)
{
    recentFiles = new RecentFilesMenu(20, 10, tr("Recent folders"), QIcon(":/images/action-clear.png"));

    ui->menu_File->insertMenu(ui->actionFileExit, recentFiles);
    ui->menu_File->insertSeparator(ui->actionFileExit);

    connect(recentFiles, SIGNAL(onFileTriggered(QString)), this, SLOT(slotRecentFiles_fileTriggered(QString)));

    recentFiles->restoreState(state);
}

void MainWindow::slotAction()
{
    QAction* action = qobject_cast<QAction*>(sender());

    if(action == ui->actionFileExit)
    {
        close();
    }
    else if(action == ui->actionFileSave)
    {
        treeModel->save();
    }
    else if(action == ui->actionFileOpenFolder)
    {
        QString newFolder = QFileDialog::getExistingDirectory(
                                    this,
                                    tr("Open folder"),
                                    currentFolder);

        if(!newFolder.isEmpty() && newFolder != currentFolder)
        {
            openFolder(newFolder);
        }
    }
    else if(action == ui->actionHelpAbout)
    {
        AboutDialog().exec();
    }
    else if(action == ui->actionRefresh)
    {
        QModelIndex index = treeModelView->currentIndex();
        if(index.isValid())
        {
            int pos = treeModelView->verticalScrollBar()->value();
            treeModel->refreshItem(index);
            treeModelView->setCurrentIndex(index);
            treeModelView->expand(index);
            treeModelView->verticalScrollBar()->setValue(pos);
        }
    }
    else if(action == ui->actionCut)
    {
        QModelIndex index = treeModelView->currentIndex();
        if(index.isValid())
        {
            treeModel->cutItem(index);
        }
    }
    else if(action == ui->actionCopy)
    {
        QModelIndex index = treeModelView->currentIndex();
        if(index.isValid())
        {
            treeModel->copyItem(index);
        }
    }
    else if(action == ui->actionPaste)
    {
        QModelIndex index = treeModelView->currentIndex();
        if(index.isValid())
        {
            treeModel->pasteIntoItem(index);
        }
    }
    else if(action == ui->actionRename)
    {
        QModelIndex index = treeModelView->currentIndex();
        if(index.isValid())
        {
            TreeItem* treeItem = treeModel->toItem(index);
            QString oldName = treeItem->getName();
            RenameDialog renameDialog(oldName);

            if(renameDialog.exec() == QDialog::Accepted)
            {
                QString newName = renameDialog.getName();

                if(newName != oldName)
                {
                    treeModel->renameItem(index, newName);
                }
            }
        }
    }
    else if(action == ui->actionEdit)
    {
        QModelIndex index = treeModelView->currentIndex();
        if(index.isValid())
        {
            treeModel->editItem(index);
        }
    }
    else if(action == ui->actionDelete)
    {
        QModelIndex index = treeModelView->currentIndex();
        if(index.isValid())
        {
            treeModel->deleteItem(index);
        }
    }
    else if(action == ui->actionMoveItemUp)
    {
        QModelIndex index = treeModelView->currentIndex();
        if(index.isValid())
        {
            treeModel->moveItemUp(index);
        }
    }
    else if(action == ui->actionMoveItemDown)
    {
        QModelIndex index = treeModelView->currentIndex();
        if(index.isValid())
        {
            treeModel->moveItemDown(index);
        }
    }
    else if(action == ui->actionAddByteTag)
    {
        addNbtTag(NBTTAG_BYTE);
    }
    else if(action == ui->actionAddShortTag)
    {
        addNbtTag(NBTTAG_SHORT);
    }
    else if(action == ui->actionAddIntTag)
    {
        addNbtTag(NBTTAG_INT);
    }
    else if(action == ui->actionAddLongTag)
    {
        addNbtTag(NBTTAG_LONG);
    }
    else if(action == ui->actionAddFloatTag)
    {
        addNbtTag(NBTTAG_FLOAT);
    }
    else if(action == ui->actionAddDoubleTag)
    {
        addNbtTag(NBTTAG_DOUBLE);
    }
    else if(action == ui->actionAddStringTag)
    {
        addNbtTag(NBTTAG_STRING);
    }
    else if(action == ui->actionAddByteArrayTag)
    {
        addNbtTag(NBTTAG_BYTE_ARRAY);
    }
    else if(action == ui->actionAddIntArrayTag)
    {
        addNbtTag(NBTTAG_INT_ARRAY);
    }
    else if(action == ui->actionAddLongArrayTag)
    {
        addNbtTag(NBTTAG_LONG_ARRAY);
    }
    else if(action == ui->actionAddListTag)
    {
        addNbtTag(NBTTAG_LIST);
    }
    else if(action == ui->actionAddCompoundTag)
    {
        addNbtTag(NBTTAG_COMPOUND);
    }
    else if(action == ui->actionFind)
    {
        QModelIndex index = treeModelView->currentIndex();
        if(index.isValid())
        {
            FindDialog dlg;

            if(dlg.exec() == FindDialog::Accepted)
            {
                lastFindItem = treeModel->toItem(index);
                lastFindIndex = index;
                lastFindName = dlg.getName();
                lastFindValue = dlg.getValue();
                lastFindPosition = -1;

                findNextItem();
            }
        }
    }
    else if(action == ui->actionFindNext)
    {
        findNextItem();
    }
    else if(action == ui->actionFindChunk)
    {
        QModelIndex index = treeModelView->currentIndex();
        if(index.isValid())
        {
            FindChunkDialog dlg;

            if(dlg.exec() == FindDialog::Accepted)
            {
                lastFindItem = nullptr;
                lastFindIndex = QModelIndex();
                lastFindName = QString();
                lastFindValue = QString();
                lastFindPosition = -1;

                QModelIndex indexFound = treeModel->findChunk(index, dlg.getChunkX(), dlg.getChunkZ());
                if(indexFound.isValid())
                {
                    treeModelView->setCurrentIndex(indexFound);
                }
                else
                {
                    QMessageBox::information(this, tr("Find chunk"), tr("Chunk not found"));
                }
            }
        }
    }
}

void MainWindow::slotModelModified()
{
    QString s = TITLE;

    if(treeModel->isModified())
    {
        s += " *";
    }
    setWindowTitle(s);
    setWindowModified(treeModel->isModified());

    updateActions();
}

void MainWindow::slotClipboard_dataChanged()
{
    updateActions();
}

void MainWindow::slotRecentFiles_fileTriggered(const QString& fileName)
{
    openFolder(fileName);
}

void MainWindow::slotTreeView_customContextMenuRequested(const QPoint& pos)
{
    Q_UNUSED(pos);

    QMenu menu;
    QModelIndex index = treeModelView->currentIndex();
    TreeItem* treeItem = treeModel->toItem(index);
    TreeItemFolder* treeItemFolder = dynamic_cast<TreeItemFolder*>(treeItem);

    if(!treeItem)
    {
        return;
    }

    if(treeItemFolder != nullptr)
    {
        if(!index.parent().isValid())
        {
            menu.addAction(actionDirUp);
        }
        if(index.parent().isValid())
        {
            menu.addAction(actionDirEnter);
        }
        menu.addSeparator();
        menu.addAction(actionOpenContainerFolder);
    }

    if(treeItem->canRefresh())
    {
        menu.addAction(ui->actionRefresh);
    }
    if(treeItem->canEdit())
    {
        menu.addAction(ui->actionEdit);
    }
    if(treeItem->canDelete())
    {
        menu.addAction(ui->actionDelete);
    }
    if(treeItem->canRename())
    {
        menu.addAction(ui->actionRename);
    }

    QList<QAction*> moveActions;
    if(treeItem->canMoveUp()) moveActions.append(ui->actionMoveItemUp);
    if(treeItem->canMoveDown()) moveActions.append(ui->actionMoveItemDown);

    QList<QAction*> addNbtTagActions;
    if(treeItem->canAddNbtTag(NBTTAG_BYTE)) addNbtTagActions.append(ui->actionAddByteTag);
    if(treeItem->canAddNbtTag(NBTTAG_SHORT)) addNbtTagActions.append(ui->actionAddShortTag);
    if(treeItem->canAddNbtTag(NBTTAG_INT)) addNbtTagActions.append(ui->actionAddIntTag);
    if(treeItem->canAddNbtTag(NBTTAG_LONG)) addNbtTagActions.append(ui->actionAddLongTag);
    if(treeItem->canAddNbtTag(NBTTAG_FLOAT)) addNbtTagActions.append(ui->actionAddFloatTag);
    if(treeItem->canAddNbtTag(NBTTAG_DOUBLE)) addNbtTagActions.append(ui->actionAddDoubleTag);
    if(treeItem->canAddNbtTag(NBTTAG_BYTE_ARRAY)) addNbtTagActions.append(ui->actionAddByteArrayTag);
    if(treeItem->canAddNbtTag(NBTTAG_STRING)) addNbtTagActions.append(ui->actionAddStringTag);
    if(treeItem->canAddNbtTag(NBTTAG_LIST)) addNbtTagActions.append(ui->actionAddListTag);
    if(treeItem->canAddNbtTag(NBTTAG_COMPOUND)) addNbtTagActions.append(ui->actionAddCompoundTag);
    if(treeItem->canAddNbtTag(NBTTAG_INT_ARRAY)) addNbtTagActions.append(ui->actionAddIntArrayTag);
    if(treeItem->canAddNbtTag(NBTTAG_LONG_ARRAY)) addNbtTagActions.append(ui->actionAddLongArrayTag);

    if(moveActions.size())
    {
        menu.addSeparator();
        menu.addActions(moveActions);
    }
    if(addNbtTagActions.size())
    {
        menu.addSeparator();
        menu.addActions(addNbtTagActions);
    }

    QAction* action = menu.exec(QCursor::pos());
    if(action == actionDirUp)
    {
        QDir dir(currentFolder);
        dir.cdUp();
        openFolder(dir.absolutePath());
    }
    else if(action == actionDirEnter)
    {
        openFolder(treeItemFolder->parentFolderPath + '/' + treeItemFolder->folderName);
    }
    else if(action == actionOpenContainerFolder)
    {
        QString folderPath = treeItemFolder->parentFolderPath + '/' + treeItemFolder->folderName;
        QString param;
        if(!QFileInfo(folderPath).isDir())
        {
            param = QLatin1String("/select,");
        }
        param += QDir::toNativeSeparators(folderPath);

        QProcess::startDetached("explorer.exe", QStringList(param));
    }
}

void MainWindow::slotTreeView_doubleClicked(const QModelIndex& index)
{
    TreeItem* item;

    if(nullptr != (item = treeModel->toItem(index)))
    {
        treeModel->editItem(index);
    }
}

void MainWindow::slotTreeView_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    updateActions();
}
