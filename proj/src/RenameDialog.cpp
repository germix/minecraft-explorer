#include "RenameDialog.h"
#include "ui_RenameDialog.h"
#include <QMessageBox>

RenameDialog::RenameDialog(const QString& initialName, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::RenameDialog)
{
    ui->setupUi(this);

    ui->txtName->setText(initialName);

    {
        QSize s = sizeHint();
        setMinimumSize(s);
        setMaximumSize(s);
        setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    }
}

RenameDialog::~RenameDialog()
{
    delete ui;
}

QString RenameDialog::getName() const
{
    return ui->txtName->text();
}

void RenameDialog::accept()
{
    if(ui->txtName->text().isEmpty())
    {
        QMessageBox::information(this, windowTitle(), tr("The name can't be empty"));
    }
    else
    {
        QDialog::accept();
    }
}
