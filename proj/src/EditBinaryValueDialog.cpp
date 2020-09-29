#include "EditBinaryValueDialog.h"
#include "ui_EditBinaryValueDialog.h"
#include "HexView.h"
#include "TreeItem.h"
#include "TreeModel.h"
#include <QByteArray>
#include <QDataStream>

EditBinaryValueDialog::EditBinaryValueDialog(TreeModel* modelIn, TreeItem* treeItemIn, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::EditBinaryValueDialog)
    , model(modelIn)
    , treeItem(treeItemIn)
{
    ui->setupUi(this);
    {
        setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    }

    hexView = new HexView();

    ui->tabWidget->addTab(hexView, tr("Hex View"));

    QString text;
    QDataStream dataStream(&currentData, QIODevice::WriteOnly);
    TreeItemNbtTagByteArray* tagByteArray;
    TreeItemNbtTagIntArray* tagIntArray;
    TreeItemNbtTagLongArray* tagLongArray;

    if(nullptr != (tagByteArray = dynamic_cast<TreeItemNbtTagByteArray*>(treeItem)))
    {
        for(int i = 0; i < tagByteArray->data.size(); i++)
        {
            text += QString::number(tagByteArray->data[i]);
            if((i+1)%16 == 0)
                text += "\n";
            else
                text += "  ";

            dataStream << tagByteArray->data[i];
        }
        setWindowTitle(tr("Edit byte array"));
        ui->lblLengthValue->setText(tr("%1 bytes").arg(tagByteArray->data.size()));
    }
    else if(nullptr != (tagIntArray = dynamic_cast<TreeItemNbtTagIntArray*>(treeItem)))
    {
        for(int i = 0; i < tagIntArray->data.size(); i++)
        {
            text += QString::number(tagIntArray->data[i]);
            if((i+1)%16 == 0)
                text += "\n";
            else
                text += "  ";

            dataStream << tagIntArray->data[i];
        }
        setWindowTitle(tr("Edit int array"));
        ui->lblLengthValue->setText(tr("%1 ints").arg(tagIntArray->data.size()));
    }
    else if(nullptr != (tagLongArray = dynamic_cast<TreeItemNbtTagLongArray*>(treeItem)))
    {
        for(int i = 0; i < tagLongArray->data.size(); i++)
        {
            text += QString::number(tagLongArray->data[i]);
            if((i+1)%16 == 0)
                text += "\n";
            else
                text += "  ";

            dataStream << tagLongArray->data[i];
        }
        setWindowTitle(tr("Edit long array"));
        ui->lblLengthValue->setText(tr("%1 longs").arg(tagLongArray->data.size()));
    }
    hexView->setData(currentData);
    ui->plainTextEdit->setPlainText(text);
    updateLength();

    QTextCursor cursor = ui->plainTextEdit->textCursor();
    cursor.setPosition(text.length(), QTextCursor::MoveAnchor);
    ui->plainTextEdit->setTextCursor(cursor);

    connect(ui->plainTextEdit, SIGNAL(textChanged()), this, SLOT(slotTextEdit_textChanged()));
}

EditBinaryValueDialog::~EditBinaryValueDialog()
{
    delete ui;
}

void EditBinaryValueDialog::updateLength()
{
    if(nullptr != dynamic_cast<TreeItemNbtTagByteArray*>(treeItem))
    {
        ui->lblLengthValue->setText(tr("%1 bytes").arg(currentData.size()));
    }
    else if(nullptr != dynamic_cast<TreeItemNbtTagIntArray*>(treeItem))
    {
        ui->lblLengthValue->setText(tr("%1 ints").arg(currentData.size()/sizeof(qint32)));
    }
    else if(nullptr != dynamic_cast<TreeItemNbtTagLongArray*>(treeItem))
    {
        ui->lblLengthValue->setText(tr("%1 longs").arg(currentData.size()/sizeof(qint64)));
    }
}

void EditBinaryValueDialog::accept()
{
    QDataStream dataStream(currentData);
    TreeItemNbtTagByteArray* tagByteArray;
    TreeItemNbtTagIntArray* tagIntArray;
    TreeItemNbtTagLongArray* tagLongArray;

    if(nullptr != (tagByteArray = dynamic_cast<TreeItemNbtTagByteArray*>(treeItem)))
    {
        tagByteArray->data.clear();
        while(!dataStream.atEnd())
        {
            qint8 value;
            dataStream >> value;
            tagByteArray->data.append(value);
        }
    }
    else if(nullptr != (tagIntArray = dynamic_cast<TreeItemNbtTagIntArray*>(treeItem)))
    {
        tagIntArray->data.clear();
        while(!dataStream.atEnd())
        {
            qint32 value;
            dataStream >> value;
            tagIntArray->data.append(value);
        }
    }
    else if(nullptr != (tagLongArray = dynamic_cast<TreeItemNbtTagLongArray*>(treeItem)))
    {
        tagLongArray->data.clear();
        while(!dataStream.atEnd())
        {
            qint64 value;
            dataStream >> value;
            tagLongArray->data.append(value);
        }
    }
    QDialog::accept();
}

void EditBinaryValueDialog::slotTextEdit_textChanged()
{
    bool failed = false;
    QStringList parts = ui->plainTextEdit->toPlainText().split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);

    currentData.clear();

    QDataStream dataStream(&currentData, QIODevice::WriteOnly);

    ui->btnOk->setEnabled(true);

    for(int i = 0; i < parts.size(); i++)
    {
        bool ok = false;
        QString s = parts[i];

        if(nullptr != dynamic_cast<TreeItemNbtTagByteArray*>(treeItem))
        {
            dataStream << (qint8)s.toShort(&ok);
        }
        else if(nullptr != dynamic_cast<TreeItemNbtTagIntArray*>(treeItem))
        {
            dataStream << s.toInt(&ok);
        }
        else if(nullptr != dynamic_cast<TreeItemNbtTagLongArray*>(treeItem))
        {
            dataStream << s.toLongLong(&ok);
        }

        if(!ok)
        {
            ui->btnOk->setEnabled(false);
            currentData.clear();
            failed = true;
            break;
        }
    }
    hexView->setData(currentData);

    if(!failed)
        updateLength();
    else
        ui->lblLengthValue->setText("---");
}
