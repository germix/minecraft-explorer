#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AboutDialog.h"

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    else if(action == ui->actionHelpAbout)
    {
        AboutDialog().exec();
    }
}
