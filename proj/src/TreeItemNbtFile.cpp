#include "TreeItem.h"
#include <QFile>

TreeItemNbtFile::TreeItemNbtFile(TreeItem* parent, const QString& folder, const QString& fileName)
    : TreeItem(parent)
{
    QFile file(folder + "/" + fileName);

    name = fileName;
    if(file.open(QFile::ReadOnly))
    {
        QByteArray data;

        data = file.read(2);
        isCompressed = (data[0] == 0x1f && data[1] == 0x8b);
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

        QDataStream stream(data);

        quint8 type;
        TreeItemNbtTag* tag;

        stream >> type;
        if(type == 0)
        {
            stream >> type;
        }
        if(type == 0)
        {
            tag = new TreeItemNbtTagEnd(this);
        }
        else
        {
            QString s = readStringUTF8(stream);

            if(!s.isEmpty())
            {
                tag = createItemTag(this, type);
                tag->name = s;
                tag->read(stream);
            }
            else
            {
                stream >> type;
                while(0 != type)
                {
                    s = readStringUTF8(stream);
                    tag = createItemTag(this, type);
                    if(tag == nullptr)
                    {
                        // TODO: error
                        break;
                    }
                    else
                    {
                        tag->name = s;
                        tag->read(stream);
                    }
                    stream >> type;
                }
            }
        }
    }
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
