#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QTreeView>
#include <QSplitter>

#include "AboutDialog.h"
#include "WorldModel.h"

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    worldModel = new WorldModel();
    worldTreeView = new QTreeView();
    worldTreeView->setModel(worldModel);
    worldTreeView->setHeaderHidden(true);
    worldTreeView->setRootIsDecorated(false);

    viewWidget = new QWidget();

    splitter = new QSplitter();
    splitter->addWidget(worldTreeView);
    splitter->addWidget(viewWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);
    splitter->setOrientation(Qt::Horizontal);
    setCentralWidget(splitter);
}

MainWindow::~MainWindow()
{
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
    worldModel->load(currentSavesFolder);
}
