#include "EditNumericValueDialog.h"
#include "ui_EditNumericValueDialog.h"
#include "TreeItem.h"
#include "TreeModel.h"
#include <QMessageBox>

EditNumericValueDialog::EditNumericValueDialog(TreeModel* modelIn, TreeItem* treeItemIn, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::EditNumericValueDialog)
    , model(modelIn)
    , treeItem(treeItemIn)
{
    ui->setupUi(this);

    if(dynamic_cast<TreeItemNbtTagByte*>(treeItem))
    {
        setWindowTitle(tr("Edit byte value"));
        ui->txtValue->setText(QString::number(((TreeItemNbtTagByte*)treeItem)->value));
    }
    else if(dynamic_cast<TreeItemNbtTagShort*>(treeItem))
    {
        setWindowTitle(tr("Edit short value"));
        ui->txtValue->setText(QString::number(((TreeItemNbtTagShort*)treeItem)->value));
    }
    else if(dynamic_cast<TreeItemNbtTagInt*>(treeItem))
    {
        setWindowTitle(tr("Edit int value"));
        ui->txtValue->setText(QString::number(((TreeItemNbtTagInt*)treeItem)->value));
    }
    else if(dynamic_cast<TreeItemNbtTagLong*>(treeItem))
    {
        setWindowTitle(tr("Edit long value"));
        ui->txtValue->setText(QString::number(((TreeItemNbtTagLong*)treeItem)->value));
    }
    else if(dynamic_cast<TreeItemNbtTagFloat*>(treeItem))
    {
        setWindowTitle(tr("Edit float value"));
        ui->txtValue->setText(QString::number(((TreeItemNbtTagFloat*)treeItem)->value));
    }
    else if(dynamic_cast<TreeItemNbtTagDouble*>(treeItem))
    {
        setWindowTitle(tr("Edit double value"));
        ui->txtValue->setText(QString::number(((TreeItemNbtTagDouble*)treeItem)->value));
    }

    {
        QSize s = sizeHint();
        setMinimumSize(s);
        setMaximumSize(s);
        setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    }
}

EditNumericValueDialog::~EditNumericValueDialog()
{
    delete ui;
}

void EditNumericValueDialog::accept()
{
    bool ok = false;
    QString text = ui->txtValue->text();

    if(!text.isEmpty())
    {
        if(dynamic_cast<TreeItemNbtTagByte*>(treeItem))
        {
            ((TreeItemNbtTagByte*)treeItem)->value = (qint8)text.toShort(&ok);
        }
        else if(dynamic_cast<TreeItemNbtTagShort*>(treeItem))
        {
            ((TreeItemNbtTagShort*)treeItem)->value = text.toShort(&ok);
        }
        else if(dynamic_cast<TreeItemNbtTagInt*>(treeItem))
        {
            ((TreeItemNbtTagInt*)treeItem)->value = text.toInt(&ok);
        }
        else if(dynamic_cast<TreeItemNbtTagLong*>(treeItem))
        {
            ((TreeItemNbtTagLong*)treeItem)->value = text.toLongLong(&ok);
        }
        else if(dynamic_cast<TreeItemNbtTagFloat*>(treeItem))
        {
            ((TreeItemNbtTagFloat*)treeItem)->value = text.toFloat(&ok);
        }
        else if(dynamic_cast<TreeItemNbtTagDouble*>(treeItem))
        {
            ((TreeItemNbtTagDouble*)treeItem)->value = text.toDouble(&ok);
        }
    }

    if(!ok)
    {
        QMessageBox::information(this, windowTitle(), tr("The value is formatted incorrectly for the given type."));
        return;
    }

    model->itemChanged(treeItem);

    QDialog::accept();
}
