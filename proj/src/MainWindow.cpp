#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QTreeView>
#include <QSplitter>
#include <QSettings>
#include <QProcess>
#include <QScrollBar>
#include <QInputDialog>

#include "AboutDialog.h"
#include "TreeModel.h"
#include "TreeItem.h"

#define TITLE "Minecraft Explorer"

#define SETTINGS_APPLICATION "MinecraftExplorer"
#define SETTINGS_ORGANIZATION "Germix"

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    actionListItemUp = new QAction(QIcon(":/images/action-move-up.png"), tr("Move up"));
    actionListItemDown = new QAction(QIcon(":/images/action-move-down.png"), tr("Move down"));
    actionDelete = new QAction(QIcon(":/images/edit-delete.png"), tr("Delete"));
    actionRename = new QAction(QIcon(":/images/action-rename.png"), tr("Rename"));
    actionRefresh = new QAction(QIcon(":/images/action-refresh.png"), tr("Refresh"));

    //
    // Load settings
    //
    QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);

    restoreGeometry(s.value("WindowGeometry").toByteArray());
    restoreState(s.value("WindowState").toByteArray());
    splitter->restoreGeometry(s.value("SplitterGeometry").toByteArray());
    splitter->restoreState(s.value("SplitterState").toByteArray());
    currentSavesFolder = s.value("SaveFolder", "").toString();

    reloadWorlds();
    updateActions();
}

MainWindow::~MainWindow()
{
    QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);
    s.setValue("WindowState", saveState());
    s.setValue("WindowGeometry", saveGeometry());
    s.setValue("SplitterState", splitter->saveState());
    s.setValue("SplitterGeometry", splitter->saveGeometry());
    s.setValue("SaveFolder", currentSavesFolder);
    delete ui;
}

void MainWindow::reloadWorlds()
{
    if(!currentSavesFolder.isEmpty())
    {
        treeModel->load(currentSavesFolder);
    }
}

void MainWindow::updateActions()
{
    ui->actionFileSave->setEnabled(treeModel->isModified());
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
        QString newSavesFolder = QFileDialog::getExistingDirectory(
                                    this,
                                    tr("Open saves folder"),
                                    currentSavesFolder);

        if(!newSavesFolder.isEmpty() && newSavesFolder != currentSavesFolder)
        {
            currentSavesFolder = newSavesFolder;

            reloadWorlds();
        }
    }
    else if(action == ui->actionHelpAbout)
    {
        AboutDialog().exec();
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

void MainWindow::slotTreeView_customContextMenuRequested(const QPoint& pos)
{
    QMenu menu;
    QModelIndex index = treeModelView->currentIndex();
    TreeItem* treeItem = treeModel->toItem(index);
    TreeItemFolder* treeItemFolder = dynamic_cast<TreeItemFolder*>(treeItem);
    TreeItemNbtTagList* parentItemTagList = dynamic_cast<TreeItemNbtTagList*>(treeItem->parent);
    TreeItemNbtTag* treeItemNbtTag = dynamic_cast<TreeItemNbtTag*>(treeItem);

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
    if(parentItemTagList != nullptr)
    {
        if(parentItemTagList->children.indexOf(treeItem) > 0)
        {
            menu.addAction(actionListItemUp);
        }
        if(parentItemTagList->children.indexOf(treeItem) < parentItemTagList->children.size()-1)
        {
            menu.addAction(actionListItemDown);
        }
    }
    if(treeItemNbtTag != nullptr)
    {
        menu.addAction(actionDelete);
    }

    if(treeItem->canRename())
    {
        menu.addAction(actionRename);
    }
    if(treeItem->canRefresh())
    {
        menu.addAction(actionRefresh);
    }

    QAction* action = menu.exec(QCursor::pos());
    if(action == actionDirUp)
    {
        QDir dir(currentSavesFolder);
        dir.cdUp();
        currentSavesFolder = dir.absolutePath();
        reloadWorlds();
    }
    else if(action == actionDirEnter)
    {
        currentSavesFolder = treeItemFolder->parentFolderPath + '/' + treeItemFolder->folderName;
        reloadWorlds();
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
    else if(action == actionListItemUp)
    {
        treeModel->moveItemUp(index);
    }
    else if(action == actionListItemDown)
    {
        treeModel->moveItemDown(index);
    }
    else if(action == actionDelete)
    {
        treeModel->deleteItem(index);
    }
    else if(action == actionRename)
    {
        QString newName = QInputDialog::getText(
                            this,
                            tr("Rename"),
                            tr("Name"),
                            QLineEdit::Normal,
                            treeItem->getName());
        if(treeItem->getName() != newName)
        {
            treeModel->renameItem(index, newName);
        }
    }
    else if(action == actionRefresh)
    {
        int pos = treeModelView->verticalScrollBar()->value();
        treeModel->refreshItem(index);
        treeModelView->setCurrentIndex(index);
        treeModelView->expand(index);
        treeModelView->verticalScrollBar()->setValue(pos);
    }
}
