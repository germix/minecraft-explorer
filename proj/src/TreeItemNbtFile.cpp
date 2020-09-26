#include "TreeItem.h"
#include <QFile>

extern void readNbtFromData(TreeItem* parent, QByteArray data);

TreeItemNbtFile::TreeItemNbtFile(TreeItem* parent, const QString& folder, const QString& fileName)
    : TreeItem(parent)
{
    QFile file(folder + "/" + fileName);

    name = fileName;
    if(file.open(QFile::ReadOnly))
    {
        QByteArray data;

        data = file.read(2);
        quint8 ch1 = (quint8)data[0];
        quint8 ch2 = (quint8)data[1];
        isCompressed = (ch1 == 0x1f && ch2 == 0x8b);
        data.clear();
        file.seek(0);
        if(!isCompressed)
        {
            data = file.readAll();
        }
        else
        {
            gzipDecompress(file.readAll(), data);
        }

        readNbtFromData(this, data);
    }
    sort();
}

QIcon TreeItemNbtFile::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-file.png");
}

QString TreeItemNbtFile::getLabel() const
{
    return name
            + " "
            + (isCompressed
                ? QObject::tr("(compressed)")
                : QObject::tr("(uncompressed)"));
}
