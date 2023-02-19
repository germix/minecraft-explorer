#include "EditStringValueDialog.h"
#include "ui_EditStringValueDialog.h"
#include "TreeItem.h"
#include "TreeModel.h"

EditStringValueDialog::EditStringValueDialog(TreeModel* modelIn, TreeItemNbtTagString* treeItemIn, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::EditStringValueDialog)
    , model(modelIn)
    , treeItem(treeItemIn)
{
    ui->setupUi(this);

    ui->plainTextEdit->setPlainText(treeItem->value);

    QTextCursor cursor = ui->plainTextEdit->textCursor();
    cursor.setPosition(treeItem->value.length(), QTextCursor::MoveAnchor);
    ui->plainTextEdit->setTextCursor(cursor);

    {
        QSize s = sizeHint();
        setMinimumSize(s);
        setMaximumSize(s);
        setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    }
}

EditStringValueDialog::~EditStringValueDialog()
{
    delete ui;
}

void EditStringValueDialog::accept()
{
    treeItem->value = ui->plainTextEdit->toPlainText();
    model->itemChanged(treeItem);
    QDialog::accept();
}
