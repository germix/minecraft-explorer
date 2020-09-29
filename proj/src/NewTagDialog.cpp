#include "NewTagDialog.h"
#include "ui_NewTagDialog.h"
#include <QMessageBox>
#include "TreeModel.h"

NewTagDialog::NewTagDialog(TreeModel* modelIn, int tagTypeIn, QModelIndex& parentIndexIn, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::NewTagDialog)
    , model(modelIn)
    , tagType(tagTypeIn)
    , parentIndex(parentIndexIn)
{
    ui->setupUi(this);

    ui->btnOk->setEnabled(false);
    connect(ui->txtName, SIGNAL(textEdited(QString)), this, SLOT(slotTextEdit_textEdited(QString)));
}

NewTagDialog::~NewTagDialog()
{
    delete ui;
}

QString NewTagDialog::getName() const
{
    return ui->txtName->text();
}

void NewTagDialog::accept()
{
    QString tagName = ui->txtName->text();

    if(model->hasChildrenWithName(parentIndex, tagName))
    {
        QMessageBox::information(this, windowTitle(), tr("Duplicate name provided."));
        return;
    }
    model->addNbtTag(parentIndex, tagType, tagName);
    QDialog::accept();
}

void NewTagDialog::slotTextEdit_textEdited(const QString& text)
{
    ui->btnOk->setEnabled(!text.isEmpty());
}
