#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QTreeView>
#include <QSplitter>
#include <QSettings>

#include "AboutDialog.h"
#include "TreeModel.h"

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

    splitter = new QSplitter();
    splitter->addWidget(treeModelView);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);
    splitter->setOrientation(Qt::Horizontal);
    setCentralWidget(splitter);

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

void MainWindow::reloadWorlds()
{
    if(!currentSavesFolder.isEmpty())
    {
        treeModel->load(currentSavesFolder);
    }
}
