#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QTreeView>
#include <QSplitter>
#include <QSettings>
#include <QProcess>

#include "AboutDialog.h"
#include "TreeModel.h"
#include "TreeItem.h"

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

    splitter = new QSplitter();
    splitter->addWidget(treeModelView);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);
    splitter->setOrientation(Qt::Horizontal);
    setCentralWidget(splitter);

    //
    // Actions
    //
    actionDirUp = new QAction(QIcon(":/images/nav-up.png"), tr("Move up"));
    actionDirEnter = new QAction(QIcon(":/images/nav-enter.png"), tr("Enter directory"));
    actionOpenContainerFolder = new QAction(QIcon(":/images/file-open-container-folder.png"), tr("Open container folder"));

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

void MainWindow::slotAction()
{
    QAction* action = qobject_cast<QAction*>(sender());

    if(action == ui->actionFileExit)
    {
        close();
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

void MainWindow::slotTreeView_customContextMenuRequested(const QPoint& pos)
{
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
}
