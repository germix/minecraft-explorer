#include "TreeItem.h"
#include <QFile>
#include "RegionFile.h"

#include <zlib.h>

TreeItemRegionChunk::TreeItemRegionChunk(TreeItemRegionFile *parent, int chunkIndexIn, int regionXIn, int regionZIn, RegionFile& regionFileIn)
    : TreeItem(parent)
    , chunkX((regionXIn*32) + (chunkIndexIn&31))
    , chunkZ((regionZIn*32) + (chunkIndexIn/32))
    , chunkIndex(chunkIndexIn)
    , localChunkX(chunkIndexIn&31)
    , localChunkZ(chunkIndexIn/32)
    , regionFile(regionFileIn)
    , canFetchData(true)
    , compressionMethod(COMPRESSION_METHOD_UNDEFINED)
{
}

TreeItem* TreeItemRegionChunk::markDirty()
{
    TreeItem* tmp = this;

    while(tmp != nullptr)
    {
        if(dynamic_cast<TreeItemRegionFile*>(tmp))
            return tmp;

        tmp = tmp->parent;
    }
    return nullptr;
}

QIcon TreeItemRegionChunk::getIcon() const
{
    return QIcon(":/images/treeitem-region-chunk.png");
}

QString TreeItemRegionChunk::getLabel() const
{
    QString s;

    s = "Chunk [" + QString::number(localChunkX) + "," + QString::number(localChunkZ) + "]";
    s += "    in world at (" + QString::number(chunkX) + "," + QString::number(chunkZ) + ")";

#if 1
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
#endif
    return s;
}

void TreeItemRegionChunk::fetchMore()
{
    canFetchData = false;

    quint32 location = regionFile.locations[chunkIndex];
    quint16 offset = location >> 8;
    quint16 length = location & 255;
    QFile& file = regionFile.file;
    QByteArray data;
    QDataStream stream(&file);
    quint32 size;
    quint8 method;

    file.seek(offset*4096);
    stream >> size;
    stream >> method;

    data.clear();
    quint32 dataLength = size - 1;
    if(method == COMPRESSION_METHOD_NONE)
    {
        data = file.read(dataLength);
    }
    else if(method == COMPRESSION_METHOD_GZIP)
    {
        data = decompressGZIP(file.read(dataLength));
    }
    else if(method == COMPRESSION_METHOD_ZLIB)
    {
        data = decompressZLIB(file.read(dataLength));
    }
    else
    {
        // TODO
    }
    if(readNbtFromData(this, data))
    {
        compressionMethod = method;
        sort();
    }
}

bool TreeItemRegionChunk::canFetchMore() const
{
    return canFetchData;
}

void TreeItemRegionChunk::saveChunk()
{
    QFile& file = regionFile.file;
    quint32 location = regionFile.locations[chunkIndex];
    quint16 offset = location >> 8;
    quint16 length = location & 255;
    QByteArray data = nbtTagsToByteArray(children, COMPRESSION_METHOD_GZIP);
    int dataSectorCount = (data.size() + 5) / 4096 + 1;

    if(dataSectorCount >= 256)
    {
        return;
    }
    if(offset != 0 && length == dataSectorCount)
    {
        QDataStream stream(&file);

        file.seek(offset*4096);
        stream << (quint32)(data.size() + 1);
        stream << (quint8)COMPRESSION_METHOD_GZIP;
        stream.writeRawData(data.constData(), data.size());
    }
    else
    {
    }
}
