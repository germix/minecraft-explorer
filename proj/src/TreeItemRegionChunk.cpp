#include "TreeItem.h"
#include <QFile>
#include <QtEndian>

extern void readNbtFromData(TreeItem* parent, QByteArray data);

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
                                         quint32 location, quint32 timestamps)
    : TreeItem(parent)
{
    //location = qFromBigEndian(location);
    quint16 offset = location >> 8;
    quint16 length = location & 255;

    chunkX = x;
    chunkZ = z;

    QByteArray data;

    file.seek(offset*4096);
    QDataStream stream(&file);
    quint32 j1; //size
    quint8 b0;

    stream >> j1;
    stream >> b0;

    data.clear();
    file.seek(offset*4096 + (4+1));
    if(b0 == 1) // GZIP
    {
        gzipDecompress(file.read(j1 - 1), data);
        readNbtFromData(this, data);
    }
    else if(b0 == 2) // ZLIB
    {
        data = gUncompress(file.read(j1 - 1));
        readNbtFromData(this, data);
    }

    sort();
}

QIcon TreeItemRegionChunk::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-compound.png");
}

QString TreeItemRegionChunk::getLabel() const
{
    return "Chunk [" + QString::number(chunkX) + "," + QString::number(chunkZ) + "]";
}
