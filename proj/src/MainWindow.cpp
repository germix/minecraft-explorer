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
#include <QCloseEvent>

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

    ui->menu_View->addAction(ui->mainToolBar->toggleViewAction());

    //
    // Init tab widget
    //
    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->setDocumentMode(true);
    ui->tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotTabWidget_currentChanged(int)));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(slotTabWidget_tabCloseRequested(int)));
    connect(ui->tabWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotTabWidget_customContextMenuRequested(QPoint)));

    //
    // Clipboard
    //
    const QClipboard* cb = QApplication::clipboard();
    connect(cb, SIGNAL(dataChanged()), this, SLOT(slotClipboard_dataChanged()));

    //
    // Load settings
    //
    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    lastTabActive = ui->tabWidget->currentIndex();
    lastOpenFolders.clear();
    for(int i = 0; i < ui->tabWidget->count(); i++)
    {
        TreeModel* treeModel = getTreeModel((QTreeView*)ui->tabWidget->widget(i));
        lastOpenFolders += treeModel->rootFolder;
    }

    if(closeAll())
        e->accept();
    else
        e->ignore();
}

void MainWindow::changeEvent(QEvent* e)
{
    if(e != NULL)
    {
        switch(e->type())
        {
            case QEvent::LocaleChange:
                {
                    QString locale = QLocale::system().name();
                    loadLanguage(locale);
                }
                break;
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                recentFiles->rebuild(tr("Recent files"));
                break;
            default:
                break;
        }
    }
    QMainWindow::changeEvent(e);
}

void MainWindow::loadSettings()
{
    QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);

    restoreGeometry(s.value("WindowGeometry").toByteArray());
    restoreState(s.value("WindowState").toByteArray());

    initRecentFilesMenu(s.value("RecentFiles").toByteArray());

    initLanguages(s.value("Language").toString());

    //
    // Reopen last open folders
    //
    lastOpenFolders = s.value("LastOpenFolders").toStringList();
    for(int i = 0; i < lastOpenFolders.size(); i++)
    {
        openFolder(lastOpenFolders[i]);
    }

    lastTabActive = s.value("LastTabActive").toInt();
    if(lastTabActive >= 0 && lastTabActive < ui->tabWidget->count())
    {
        ui->tabWidget->setCurrentIndex(lastTabActive);
    }

    lastTabActive = -1;
    lastOpenFolders.clear();

    updateActions();
}

void MainWindow::saveSettings()
{
    QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);
    s.setValue("WindowState", saveState());
    s.setValue("WindowGeometry", saveGeometry());
    s.setValue("RecentFiles", recentFiles->saveState());
    s.setValue("LastFolder", lastFolder);
    s.setValue("LastTabActive", lastTabActive);
    s.setValue("LastOpenFolders", lastOpenFolders);
}

void MainWindow::updateActions()
{
    QTreeView* treeView = currentTreeView();
    if(treeView)
    {
        TreeModel* treeModel = getTreeModel(treeView);

        ui->actionFileSave->setEnabled(treeModel->isModified());

        QModelIndex index = treeView->currentIndex();
        TreeItem* item = treeModel->toItem(index);
        if(item != nullptr)
        {
            FindData* fd = getFindData(treeModel);
            ui->actionFind->setEnabled(item->canFind());
            ui->actionFindNext->setEnabled(item == getFindData(treeModel)->lastFindItem);
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

            return;
        }
    }

    ui->actionFind->setEnabled(false);
    ui->actionFindNext->setEnabled(false);
    ui->actionFindChunk->setEnabled(false);

    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);

    ui->actionEdit->setEnabled(false);
    ui->actionDelete->setEnabled(false);
    ui->actionRename->setEnabled(false);
    ui->actionRefresh->setEnabled(false);
    ui->actionMoveItemUp->setEnabled(false);
    ui->actionMoveItemDown->setEnabled(false);

    checkNbtTag(nullptr, ui->actionAddByteTag, NBTTAG_BYTE);
    checkNbtTag(nullptr, ui->actionAddShortTag, NBTTAG_SHORT);
    checkNbtTag(nullptr, ui->actionAddIntTag, NBTTAG_INT);
    checkNbtTag(nullptr, ui->actionAddLongTag, NBTTAG_LONG);
    checkNbtTag(nullptr, ui->actionAddFloatTag, NBTTAG_FLOAT);
    checkNbtTag(nullptr, ui->actionAddDoubleTag, NBTTAG_DOUBLE);
    checkNbtTag(nullptr, ui->actionAddByteArrayTag, NBTTAG_BYTE_ARRAY);
    checkNbtTag(nullptr, ui->actionAddStringTag, NBTTAG_STRING);
    checkNbtTag(nullptr, ui->actionAddListTag, NBTTAG_LIST);
    checkNbtTag(nullptr, ui->actionAddCompoundTag, NBTTAG_COMPOUND);
    checkNbtTag(nullptr, ui->actionAddIntArrayTag, NBTTAG_INT_ARRAY);
    checkNbtTag(nullptr, ui->actionAddLongArrayTag, NBTTAG_LONG_ARRAY);
}

void MainWindow::updateTabLabel()
{
    QTreeView* treeView = currentTreeView();
    if(!treeView)
        return;
    TreeModel* treeModel = getTreeModel(treeView);
    QString s = treeModel->rootName;
    bool isModified = treeView != nullptr && treeModel->isModified();

    if(isModified)
    {
        s += " *";
    }

    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), s);
}

void MainWindow::addNbtTag(int type)
{
    QTreeView* treeView = currentTreeView();
    TreeModel* treeModel = getTreeModel(treeView);
    QModelIndex index = treeView->currentIndex();

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
    if(parent == nullptr)
    {
        action->setVisible(false);
        action->setEnabled(false);
    }
    else
    {
        action->setVisible(parent->canAddNbtTag(type));
        action->setEnabled(parent->canAddNbtTag(type));
    }
}

void MainWindow::findNextItem()
{
    QTreeView* treeView = currentTreeView();
    if(!treeView)
        return;
    TreeModel* treeModel = getTreeModel(treeView);
    FindData* fd = getFindData(treeModel);

    if(fd->lastFindIndex.isValid())
    {
        QModelIndex indexFound = treeModel->findItem(fd->lastFindIndex, fd->lastFindPosition+1, fd->lastFindName, fd->lastFindValue);
        if(indexFound.isValid())
        {
            fd->lastFindItem = treeModel->toItem(indexFound);
            fd->lastFindIndex = indexFound.parent();
            fd->lastFindPosition = indexFound.row();
            treeView->setCurrentIndex(indexFound);
        }
        else
        {
            fd->clear();
            updateActions();
            QMessageBox::information(this, tr("Find"), tr("Item not found"));
        }
    }
}

void MainWindow::openFolder(const QString& folder)
{
    if(folder.isEmpty())
        return;
    QFileInfo fi(folder);
    QString absoluteFolder = fi.absoluteFilePath();

    if(mapFolderToTree.contains(absoluteFolder))
    {
        QTreeView* widget = mapFolderToTree.find(absoluteFolder).value();

        ui->tabWidget->setCurrentWidget(widget);
    }
    else
    {
        recentFiles->removeFile(absoluteFolder);
        lastFolder = absoluteFolder;

        QTreeView* treeView = new QTreeView();
        TreeModel* treeModel = new TreeModel();

        treeView->setModel(treeModel);
        treeView->setHeaderHidden(true);
        treeView->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(treeView,
                SIGNAL(customContextMenuRequested(QPoint)),
                this,
                SLOT(slotTreeView_customContextMenuRequested(QPoint)));
        connect(treeView->selectionModel(),
            SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,
            SLOT(slotTreeView_currentChanged(QModelIndex,QModelIndex)));
        connect(treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotTreeView_doubleClicked(QModelIndex)));
        
        connect(treeModel, SIGNAL(onModified()), this, SLOT(slotModelModified()));

        // Open folder
        treeModel->loadFolder(absoluteFolder);
        treeView->expand(treeModel->firstIndex());

        mapFolderToTree.insert(absoluteFolder, treeView);
        mapModelToFindData.insert(treeModel, new FindData());

        int tabIndex = ui->tabWidget->addTab(treeView, QIcon(":/images/treeitem-world-folder.png"), treeModel->rootName);
        ui->tabWidget->setTabToolTip(tabIndex, treeModel->rootFolder);

        ui->tabWidget->setCurrentWidget(treeView);
    }
    updateActions();
}

void MainWindow::enterFolder(QTreeView* treeView, TreeModel* treeModel, const QString& folder)
{
    treeModel->enterFolder(QFileInfo(folder).absoluteFilePath());
    treeView->expand(treeModel->firstIndex());
}

void MainWindow::initRecentFilesMenu(const QByteArray& state)
{
    recentFiles = new RecentFilesMenu(20, 10, tr("Recent folders"), QIcon(":/images/action-clear.png"));

    ui->menu_File->insertMenu(ui->actionFileExit, recentFiles);
    ui->menu_File->insertSeparator(ui->actionFileExit);

    connect(recentFiles, SIGNAL(onFileTriggered(QString)), this, SLOT(slotRecentFiles_fileTriggered(QString)));

    recentFiles->restoreState(state);
}

void MainWindow::loadLanguage(QString language)
{
    if(currentLanguage != language)
    {
        currentLanguage = language;

        QLocale locale = QLocale(language);
        QLocale::setDefault(locale);

        QString fileName = languagesPath + "/" + QString("mcexplorer_%1.qm").arg(language);

        // Remove old translator
        qApp->removeTranslator(&currentTranslator);

        // Load new translator
        if(currentTranslator.load(fileName))
        {
            qApp->installTranslator(&currentTranslator);
        }
    }
}

void MainWindow::initLanguages(QString initialLocale)
{
    QActionGroup* langGroup = new QActionGroup(this);
    langGroup->setExclusive(true);

    connect(langGroup, SIGNAL(triggered(QAction*)), this, SLOT(slotLanguageChanged(QAction*)));

    //
    // Si no hay un lenguaje inicial, usar el del sistema
    //
    if(initialLocale.isEmpty())
    {
        QString defaultLocale = QLocale::system().name();
        defaultLocale.truncate(defaultLocale.lastIndexOf('_'));

        initialLocale = defaultLocale;
    }

    //
    // Obtener la carpeta de donde están los lenguajes
    //
    languagesPath = QApplication::applicationDirPath();
    languagesPath.append("/translations");

    //
    // Obtener el listado de los archivos de lenguaje
    //
    QDir dir(languagesPath);
    QStringList fileNames = dir.entryList(QStringList("mcexplorer_*.qm"));

    //
    // Enumarar cada uno e ir creando una acción para ir agregandola al menú
    //
    for(int i = 0; i < fileNames.size(); i++)
    {
        QString locale;

        locale = fileNames[i];
        locale.truncate(locale.lastIndexOf('.'));
        locale.remove(0, locale.indexOf('_') + 1);

        QString lang = QLocale::languageToString(QLocale(locale).language());
        QIcon ico(QString("%1/%2.png").arg(languagesPath).arg(locale));

        QAction* action = new QAction(ico, lang, this);
        action->setCheckable(true);
        action->setData(locale);

        ui->menuSettingsLanguage->addAction(action);
        langGroup->addAction(action);

        if(initialLocale == locale)
        {
            action->setChecked(true);
        }
    }
    //
    // Cargar lenguaje inicial
    //
    loadLanguage(initialLocale);
}

bool MainWindow::closeAll()
{
    QTreeView* tree;

    while(NULL != (tree = currentTreeView()))
    {
        if(!closeEditor(tree))
            return false;
    }
    return true;
}

bool MainWindow::closeEditor(QTreeView* editor)
{
    TreeModel* treeModel = getTreeModel(editor);

    if(checkForUnsavedChanges(treeModel))
    {
        // Update recent folders
        if(!treeModel->rootFolder.isEmpty())
            recentFiles->addFile(treeModel->rootFolder);

        // Remove treeview from tab widget
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(editor));

        // Cleanup
        mapFolderToTree.remove(treeModel->rootFolder);
        delete mapModelToFindData.find(treeModel).value();
        mapModelToFindData.remove(treeModel);

        // Update actions
        updateActions();

        return true;
    }
    return false;
}

bool MainWindow::checkForUnsavedChanges(TreeModel* treeModel)
{
    if(treeModel->isModified())
    {
        switch(QMessageBox::question(this,
                tr("Unsaved changes"),
                tr("There are unsaved changes. Close anyway?"),
                QMessageBox::Save | QMessageBox::Yes | QMessageBox::Cancel))
        {
            case QMessageBox::Save:
                treeModel->save();
                break;
            case QMessageBox::Yes:
                break;
            case QMessageBox::Cancel:
                return false;
            default:
                break;
        }
    }
    return true;
}

TreeModel* MainWindow::getTreeModel(QTreeView* treeView)
{
    if(treeView != nullptr)
    {
        return (TreeModel*)treeView->model();
    }
    return nullptr;
}

QTreeView* MainWindow::currentTreeView()
{
    return (QTreeView*)ui->tabWidget->currentWidget();
}

FindData* MainWindow::getFindData(TreeModel* treeModel)
{
    return mapModelToFindData.find(treeModel).value();
}

void MainWindow::slotAction()
{
    QAction* action = qobject_cast<QAction*>(sender());
    QTreeView* treeView = currentTreeView();
    TreeModel* treeModel = getTreeModel(treeView);

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
                                    lastFolder);

        if(!newFolder.isEmpty())
        {
            if(TreeModel::isWorldFolder(newFolder))
            {
                openFolder(newFolder);
            }
            else
            {
                QMessageBox::information(this, tr("Error"), tr("The folder is not a valid world folder. Does not contains \"level.dat\""), QMessageBox::Information);
            }
        }
    }
    else if(action == ui->actionClose)
    {
        closeEditor(currentTreeView());
    }
    else if(action == ui->actionCloseAll)
    {
        closeAll();
    }
    else if(action == ui->actionHelpAbout)
    {
        AboutDialog().exec();
    }
    else if(action == ui->actionRefresh)
    {
        QModelIndex index = treeView->currentIndex();
        if(index.isValid())
        {
            int pos = treeView->verticalScrollBar()->value();
            treeModel->refreshItem(index);
            treeView->setCurrentIndex(index);
            treeView->expand(index);
            treeView->verticalScrollBar()->setValue(pos);
        }
    }
    else if(action == ui->actionCut)
    {
        QModelIndex index = treeView->currentIndex();
        if(index.isValid())
        {
            treeModel->cutItem(index);
        }
    }
    else if(action == ui->actionCopy)
    {
        QModelIndex index = treeView->currentIndex();
        if(index.isValid())
        {
            treeModel->copyItem(index);
        }
    }
    else if(action == ui->actionPaste)
    {
        QModelIndex index = treeView->currentIndex();
        if(index.isValid())
        {
            treeModel->pasteIntoItem(index);
        }
    }
    else if(action == ui->actionRename)
    {
        QModelIndex index = treeView->currentIndex();
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
        QModelIndex index = treeView->currentIndex();
        if(index.isValid())
        {
            treeModel->editItem(index);
        }
    }
    else if(action == ui->actionDelete)
    {
        QModelIndex index = treeView->currentIndex();
        if(index.isValid())
        {
            treeModel->deleteItem(index);
        }
    }
    else if(action == ui->actionMoveItemUp)
    {
        QModelIndex index = treeView->currentIndex();
        if(index.isValid())
        {
            treeModel->moveItemUp(index);
        }
    }
    else if(action == ui->actionMoveItemDown)
    {
        QModelIndex index = treeView->currentIndex();
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
        QModelIndex index = treeView->currentIndex();
        if(index.isValid())
        {
            FindDialog dlg;

            if(dlg.exec() == FindDialog::Accepted)
            {
                FindData* fd = mapModelToFindData.find(treeModel).value();
                fd->lastFindItem = treeModel->toItem(index);
                fd->lastFindIndex = index;
                fd->lastFindName = dlg.getName();
                fd->lastFindValue = dlg.getValue();
                fd->lastFindPosition = -1;

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
        QModelIndex index = treeView->currentIndex();
        if(index.isValid())
        {
            FindChunkDialog dlg;

            if(dlg.exec() == FindDialog::Accepted)
            {
                getFindData(treeModel)->clear();

                QModelIndex indexFound = treeModel->findChunk(index, dlg.getChunkX(), dlg.getChunkZ());
                if(indexFound.isValid())
                {
                    treeView->setCurrentIndex(indexFound);
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
    updateActions();
    updateTabLabel();
}

void MainWindow::slotLanguageChanged(QAction* action)
{
    if(action != NULL)
    {
        loadLanguage(action->data().toString());
    }
}

void MainWindow::slotClipboard_dataChanged()
{
    updateActions();
}

void MainWindow::slotRecentFiles_fileTriggered(const QString& fileName)
{
    openFolder(fileName);
}

void MainWindow::slotTabWidget_currentChanged(int index)
{
    updateActions();
    slotModelModified();
}

void MainWindow::slotTabWidget_tabCloseRequested(int index)
{
    if(index != -1)
    {
        closeEditor((QTreeView*)ui->tabWidget->widget(index));
    }
}

void MainWindow::slotTreeView_customContextMenuRequested(const QPoint& pos)
{
    Q_UNUSED(pos);
    QTreeView* treeView = currentTreeView();
    if(!treeView)
        return;
    TreeModel* treeModel = getTreeModel(treeView);
    QMenu menu;
    QModelIndex index = treeView->currentIndex();
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
            if(treeModel->rootFolder != treeModel->currentFolder)
                menu.addAction(ui->actionDirUp);
        }
        if(index.parent().isValid())
        {
            menu.addAction(ui->actionDirEnter);
        }
        menu.addSeparator();
        menu.addAction(ui->actionOpenContainerFolder);
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
    if(action == ui->actionDirUp)
    {
        QDir dir(treeModel->currentFolder);
        dir.cdUp();
        enterFolder(treeView, treeModel, dir.absolutePath());
    }
    else if(action == ui->actionDirEnter)
    {
        enterFolder(treeView, treeModel, treeItemFolder->parentFolderPath + '/' + treeItemFolder->folderName);
    }
    else if(action == ui->actionOpenContainerFolder)
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

void MainWindow::slotTabWidget_customContextMenuRequested(const QPoint& pos)
{
    int index;

    if(-1 != (index = ui->tabWidget->tabBar()->tabAt(pos)))
    {
        ui->tabWidget->setCurrentIndex(index);

        QMenu menu;

        menu.addAction(ui->actionClose);
        menu.addAction(ui->actionCloseAll);

        menu.exec(QCursor::pos());
    }
}

void MainWindow::slotTreeView_doubleClicked(const QModelIndex& index)
{
    QTreeView* treeView = currentTreeView();
    if(!treeView)
        return;
    TreeModel* treeModel = getTreeModel(treeView);
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
