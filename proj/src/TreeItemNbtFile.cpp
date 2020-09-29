#include "TreeItem.h"
#include <QFile>

TreeItemNbtFile::TreeItemNbtFile(TreeItem* parent, const QString& fileNameIn, const QString& parentFolderPathIn)
    : TreeItem(parent)
    , canFetchData(true)
    , fileName(fileNameIn)
    , parentFolderPath(parentFolderPathIn)
    , compressionMethod(COMPRESSION_METHOD_UNDEFINED)
{
}

QIcon TreeItemNbtFile::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-file.png");
}

QString TreeItemNbtFile::getLabel() const
{
    QString s = fileName;

    if(compressionMethod != COMPRESSION_METHOD_UNDEFINED)
    {
        s += " ";
        switch(compressionMethod)
        {
            case COMPRESSION_METHOD_NONE:
                s += QObject::tr("(uncompressed)");
                break;
            case COMPRESSION_METHOD_GZIP:
                s += QObject::tr("(compressed with GZIP)");
                break;
            case COMPRESSION_METHOD_ZLIB:
                s += QObject::tr("(compressed with ZLIB)");
                break;
        }
    }
    return s;
}

void TreeItemNbtFile::fetchMore()
{
    canFetchData = false;

    QFile file(parentFolderPath + "/" + fileName);
    if(file.open(QFile::ReadOnly))
    {
        QByteArray data;

        data = file.read(2);
        quint8 ch1 = (quint8)data[0];
        quint8 ch2 = (quint8)data[1];
        quint8 method = (ch1 == 0x1f && ch2 == 0x8b)
                    ? COMPRESSION_METHOD_GZIP
                    : COMPRESSION_METHOD_NONE;

        file.seek(0);
        if(method == COMPRESSION_METHOD_NONE)
        {
            data = file.readAll();
        }
        else
        {
            data = decompressGZIP(file.readAll());
        }

        if(readNbtFromData(this, data))
        {
            compressionMethod = method;
            sort();
        }
    }
}

QByteArray nbtTagsToByteArray(QVector<TreeItem*>& children, int compressionMethod)
{
    QByteArray data;
    QDataStream dataStream(&data, QIODevice::WriteOnly);

    dataStream << (quint8)NBTTAG_COMPOUND;
    writeStringUTF8("", dataStream);

    for(int i = 0; i < children.size(); i++)
    {
        TreeItemNbtTag* tag = (TreeItemNbtTag*)children[i];

        dataStream << (quint8)tag->nbtType();
        writeStringUTF8(tag->name, dataStream);
        tag->writeNbt(dataStream);
    }
    dataStream << (quint8)NBTTAG_END;

    if(compressionMethod == COMPRESSION_METHOD_GZIP)
    {
        return compressGZIP(data, 9);
    }
    if(compressionMethod == COMPRESSION_METHOD_ZLIB)
    {
        return decompressZLIB(data);
    }

    return data;
}

bool TreeItemNbtFile::canFetchMore() const
{
    return canFetchData;
}

void TreeItemNbtFile::saveItem()
{
    QFile file(parentFolderPath + "/" + fileName);
    if(file.open(QFile::WriteOnly))
    {
        file.write(nbtTagsToByteArray(children, COMPRESSION_METHOD_NONE));
    }
}

TreeItem* TreeItemNbtFile::markDirty()
{
    return this;
}
