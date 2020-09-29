#include "FindDialog.h"
#include "ui_FindDialog.h"

FindDialog::FindDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::FindDialog)
{
    ui->setupUi(this);

    connect(ui->txtName, SIGNAL(textChanged(QString)), this, SLOT(slotTextEditName_textChanged(QString)));
    connect(ui->txtValue, SIGNAL(textChanged(QString)), this, SLOT(slotTextEditValue_textChanged(QString)));
}

FindDialog::~FindDialog()
{
    delete ui;
}

QString FindDialog::getName() const
{
    if(!ui->chkName->isChecked())
        return QString();
    return ui->txtName->text();
}

QString FindDialog::getValue() const
{
    if(!ui->chkValue->isChecked())
        return QString();
    return ui->txtValue->text();
}

void FindDialog::slotTextEditName_textChanged(const QString& text)
{
    ui->chkName->setChecked(true);
}

void FindDialog::slotTextEditValue_textChanged(const QString& text)
{
    ui->chkValue->setChecked(true);
}
