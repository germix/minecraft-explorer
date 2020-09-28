#include "TreeItem.h"
#include <QFile>
#include "RegionFile.h"

#include <zlib.h>

// https://stackoverflow.com/questions/2690328/qt-quncompress-gzip-data
QByteArray gUncompress(const QByteArray &data)
{
    if (data.size() <= 4) {
        qWarning("gUncompress: Input data is truncated");
        return QByteArray();
    }

    QByteArray result;

    int ret;
    z_stream strm;
    static const int CHUNK_SIZE = 1024;
    char out[CHUNK_SIZE];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = data.size();
    strm.next_in = (Bytef*)(data.data());

    ret = inflateInit2(&strm, 15 +  32); // gzip decoding
    if (ret != Z_OK)
        return QByteArray();

    // run inflate()
    do {
        strm.avail_out = CHUNK_SIZE;
        strm.next_out = (Bytef*)(out);

        ret = inflate(&strm, Z_NO_FLUSH);
        Q_ASSERT(ret != Z_STREAM_ERROR);  // state not clobbered

        switch (ret) {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR;     // and fall through
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            return QByteArray();
        }

        result.append(out, CHUNK_SIZE - strm.avail_out);
    } while (strm.avail_out == 0);

    // clean up and return
    inflateEnd(&strm);
    return result;
}

TreeItemRegionChunk::TreeItemRegionChunk(TreeItemRegionFile *parent, int chunkIndexIn, RegionFile& regionFileIn)
    : TreeItem(parent)
    , chunkX(chunkIndexIn&31)
    , chunkZ(chunkIndexIn/32)
    , chunkIndex(chunkIndexIn)
    , regionFile(regionFileIn)
    , canFetchData(true)
    , compressionMethod(COMPRESSION_METHOD_UNDEFINED)
    , modified(false)
{
}

QIcon TreeItemRegionChunk::getIcon() const
{
    return QIcon(":/images/treeitem-region-chunk.png");
}

QString TreeItemRegionChunk::getLabel() const
{
    QString s;

    s = "Chunk [" + QString::number(chunkX) + "," + QString::number(chunkZ) + "]";

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
        gzipDecompress(file.read(dataLength), data);
    }
    else if(method == COMPRESSION_METHOD_ZLIB)
    {
        data = gUncompress(file.read(dataLength));
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
    int k1 = (data.size() + 5) / 4096 + 1;

    if(k1 >= 256)
    {
        return;
    }
    if(offset != 0 && length == k1)
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

    modified = false;
}
