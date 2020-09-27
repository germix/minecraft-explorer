#include "TreeItem.h"
#include <QFile>
#include <QtEndian>

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

TreeItemRegionChunk::TreeItemRegionChunk(TreeItemRegionFile *parent,
                                         QFile& file,
                                         int x,
                                         int z,
                                         quint32 locationIn,
                                         quint32 timestampIn)
    : TreeItem(parent)
    , location(locationIn)
    , timestamp(timestampIn)
    , compressionMethod(COMPRESSION_METHOD_UNDEFINED)
{
    quint16 offset = location >> 8;
    quint16 length = location & 255;

    chunkX = x;
    chunkZ = z;

    file.seek(offset*4096);
    QByteArray data;
    QDataStream stream(&file);
    quint32 size;
    quint8 method;

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

QIcon TreeItemRegionChunk::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-compound.png");
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
