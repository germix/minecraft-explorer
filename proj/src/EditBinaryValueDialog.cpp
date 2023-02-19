#include "EditBinaryValueDialog.h"
#include "ui_EditBinaryValueDialog.h"
#include "HexView.h"
#include "TreeItem.h"
#include "TreeModel.h"
#include <QByteArray>
#include <QDataStream>
#include <QFileDialog>

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
    }
    hexView->setData(currentData);
    ui->plainTextEdit->setPlainText(text);
    updateLength();

    QTextCursor cursor = ui->plainTextEdit->textCursor();
    cursor.setPosition(text.length(), QTextCursor::MoveAnchor);
    ui->plainTextEdit->setTextCursor(cursor);

    connect(ui->plainTextEdit, SIGNAL(textChanged()), this, SLOT(slotTextEdit_textChanged()));

    dataFilters << tr("Text data (*.txt)");
    dataFilters << tr("Binary data (*.bin)");
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

void EditBinaryValueDialog::importTextData(QFile& file)
{
    ui->plainTextEdit->setPlainText(QString::fromUtf8(file.readAll()));
    updateDataFromText();
}

void EditBinaryValueDialog::importBinaryData(QFile& file)
{
    currentData = file.readAll();
    QString text;

    if(dataToText(currentData, &text))
    {
        ui->plainTextEdit->setPlainText(text);
        updateLength();
    }
    else
    {
        ui->lblLengthValue->setText("---");
    }
}

void EditBinaryValueDialog::exportAsTextData(QFile& file)
{
    QString text;

    if(dataToText(currentData, &text))
    {
        file.write(text.toUtf8());
    }
}

void EditBinaryValueDialog::exportAsBinaryData(QFile& file)
{
    file.write(currentData);
}

bool EditBinaryValueDialog::dataToText(QByteArray data, QString* output)
{
    int i = 0;
    QString text;
    QDataStream dataStream(data);
    TreeItemNbtTagByteArray* tagByteArray;
    TreeItemNbtTagIntArray* tagIntArray;
    TreeItemNbtTagLongArray* tagLongArray;

    if(nullptr != (tagByteArray = dynamic_cast<TreeItemNbtTagByteArray*>(treeItem)))
    {
        while(!dataStream.atEnd())
        {
            qint8 value;
            dataStream >> value;
            text += QString::number(value);
            if((++i)%16 == 0)
                text += "\n";
            else
                text += "  ";
        }
    }
    else if(nullptr != (tagIntArray = dynamic_cast<TreeItemNbtTagIntArray*>(treeItem)))
    {
        while(!dataStream.atEnd())
        {
            qint32 value;
            dataStream >> value;
            text += QString::number(value);
            if((++i)%16 == 0)
                text += "\n";
            else
                text += "  ";
        }
    }
    else if(nullptr != (tagLongArray = dynamic_cast<TreeItemNbtTagLongArray*>(treeItem)))
    {
        while(!dataStream.atEnd())
        {
            qint64 value;
            dataStream >> value;
            text += QString::number(value);
            if((++i)%16 == 0)
                text += "\n";
            else
                text += "  ";
        }
    }
    *output = text;

    return true;
}

void EditBinaryValueDialog::updateDataFromText()
{
    bool failed = false;
    QStringList parts = ui->plainTextEdit->toPlainText().split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);

    currentData.clear();

    QDataStream dataStream(&currentData, QIODevice::WriteOnly);

    ui->btnOk->setEnabled(true);
    ui->btnExport->setEnabled(true);

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
            ui->btnExport->setEnabled(false);
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

void EditBinaryValueDialog::updateTextFromData()
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
    model->itemChanged(treeItem);
    QDialog::accept();
}

void EditBinaryValueDialog::slotImport()
{
    QString fileName = QFileDialog::getOpenFileName(
                    this,
                    tr("Import"),
                    QString(),
                    dataFilters.join(";;"));

    if(!fileName.isEmpty())
    {
        QFileInfo info(fileName);
        QFile file(fileName);

        if(file.open(QFile::ReadOnly))
        {
            if(info.suffix() == "txt")
            {
                importTextData(file);
            }
            else if(info.suffix() == "bin")
            {
                importBinaryData(file);
            }
        }
    }
}

void EditBinaryValueDialog::slotExport()
{
    QString fileName = QFileDialog::getSaveFileName(
                    this,
                    tr("Export"),
                    QString(),
                    dataFilters.join(";;"));

    if(!fileName.isEmpty())
    {
        QFileInfo info(fileName);
        QFile file(fileName);

        if(file.open(QFile::WriteOnly))
        {
            if(info.suffix() == "txt")
            {
                exportAsTextData(file);
            }
            else if(info.suffix() == "bin")
            {
                exportAsBinaryData(file);
            }
        }
    }
}

void EditBinaryValueDialog::slotTextEdit_textChanged()
{
    updateDataFromText();
}
