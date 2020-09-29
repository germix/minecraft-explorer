#include "utils.h"
#include "TreeItem.h"

#include <QDir>
#include <QFile>
#include <QDataStream>

#include <zlib.h>


#define GZIP_WINDOWS_BIT 15 + 16
#define GZIP_CHUNK_SIZE 32 * 1024

//
// https://stackoverflow.com/questions/2690328/qt-quncompress-gzip-data
//


/**
 * @brief Compresses the given buffer using the standard GZIP algorithm
 * @param input The buffer to be compressed
 * @param output The result of the compression
 * @param level The compression level to be used (@c 0 = no compression, @c 9 = max, @c -1 = default)
 * @return @c true if the compression was successful, @c false otherwise
 */
bool gzipCompress(QByteArray input, QByteArray &output, int level)
{
    // Prepare output
    output.clear();

    // Is there something to do?
    if(input.length())
    {
        // Declare vars
        int flush = 0;

        // Prepare deflater status
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;

        // Initialize deflater
        int ret = deflateInit2(&strm, qMax(-1, qMin(9, level)), Z_DEFLATED, GZIP_WINDOWS_BIT, 8, Z_DEFAULT_STRATEGY);

        if (ret != Z_OK)
            return(false);

        // Prepare output
        output.clear();

        // Extract pointer to input data
        char *input_data = input.data();
        int input_data_left = input.length();

        // Compress data until available
        do {
            // Determine current chunk size
            int chunk_size = qMin(GZIP_CHUNK_SIZE, input_data_left);

            // Set deflater references
            strm.next_in = (unsigned char*)input_data;
            strm.avail_in = chunk_size;

            // Update interval variables
            input_data += chunk_size;
            input_data_left -= chunk_size;

            // Determine if it is the last chunk
            flush = (input_data_left <= 0 ? Z_FINISH : Z_NO_FLUSH);

            // Deflate chunk and cumulate output
            do {

                // Declare vars
                char out[GZIP_CHUNK_SIZE];

                // Set deflater references
                strm.next_out = (unsigned char*)out;
                strm.avail_out = GZIP_CHUNK_SIZE;

                // Try to deflate chunk
                ret = deflate(&strm, flush);

                // Check errors
                if(ret == Z_STREAM_ERROR)
                {
                    // Clean-up
                    deflateEnd(&strm);

                    // Return
                    return(false);
                }

                // Determine compressed size
                int have = (GZIP_CHUNK_SIZE - strm.avail_out);

                // Cumulate result
                if(have > 0)
                    output.append((char*)out, have);

            } while (strm.avail_out == 0);

        } while (flush != Z_FINISH);

        // Clean-up
        (void)deflateEnd(&strm);

        // Return
        return(ret == Z_STREAM_END);
    }
    else
        return(true);
}

/**
 * @brief Decompresses the given buffer using the standard GZIP algorithm
 * @param input The buffer to be decompressed
 * @param output The result of the decompression
 * @return @c true if the decompression was successfull, @c false otherwise
 */
bool gzipDecompress(QByteArray input, QByteArray &output)
{
    // Prepare output
    output.clear();

    // Is there something to do?
    if(input.length() > 0)
    {
        // Prepare inflater status
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;

        // Initialize inflater
        int ret = inflateInit2(&strm, GZIP_WINDOWS_BIT);

        if (ret != Z_OK)
            return(false);

        // Extract pointer to input data
        char *input_data = input.data();
        int input_data_left = input.length();

        // Decompress data until available
        do {
            // Determine current chunk size
            int chunk_size = qMin(GZIP_CHUNK_SIZE, input_data_left);

            // Check for termination
            if(chunk_size <= 0)
                break;

            // Set inflater references
            strm.next_in = (unsigned char*)input_data;
            strm.avail_in = chunk_size;

            // Update interval variables
            input_data += chunk_size;
            input_data_left -= chunk_size;

            // Inflate chunk and cumulate output
            do {

                // Declare vars
                char out[GZIP_CHUNK_SIZE];

                // Set inflater references
                strm.next_out = (unsigned char*)out;
                strm.avail_out = GZIP_CHUNK_SIZE;

                // Try to inflate chunk
                ret = inflate(&strm, Z_NO_FLUSH);

                switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                case Z_STREAM_ERROR:
                    // Clean-up
                    inflateEnd(&strm);

                    // Return
                    return(false);
                }

                // Determine decompressed size
                int have = (GZIP_CHUNK_SIZE - strm.avail_out);

                // Cumulate result
                if(have > 0)
                    output.append((char*)out, have);

            } while (strm.avail_out == 0);

        } while (ret != Z_STREAM_END);

        // Clean-up
        inflateEnd(&strm);

        // Return
        return (ret == Z_STREAM_END);
    }
    else
        return(true);
}

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

QByteArray compressGZIP(QByteArray input, int level)
{
    QByteArray compressed;
    gzipCompress(input, compressed, level);
    return compressed;
}

QByteArray decompressGZIP(QByteArray data)
{
    QByteArray decompressedData;
    gzipDecompress(data, decompressedData);
    return decompressedData;
}

QByteArray compressZLIB(QByteArray data)
{
    QByteArray compressedData = qCompress(data);
    compressedData.remove(0, 4);
    return compressedData;
}

QByteArray decompressZLIB(QByteArray data)
{
    return gUncompress(data);
}

QString readStringUTF8(QDataStream& in)
{
    quint16 utflen;
    QString utfstr;

    in >> utflen;
    char* data = (char*)malloc(utflen);
    in.readRawData(data, utflen);

    utfstr = QString::fromUtf8(data, utflen);

    free(data);
    return utfstr;
}

void writeStringUTF8(const QString& src, QDataStream& out)
{
    QByteArray data = src.toUtf8();
    quint16 len16 = data.size();

    out << len16;
    if(len16 > 0)
    {
        out.writeRawData(data.constData(), data.length());
    }
}

TreeItemNbtTag* createItemTag(TreeItem* parent, quint8 type)
{
    switch(type)
    {
        case NBTTAG_BYTE: return new TreeItemNbtTagByte(parent);
        case NBTTAG_SHORT: return new TreeItemNbtTagShort(parent);
        case NBTTAG_INT: return new TreeItemNbtTagInt(parent);
        case NBTTAG_LONG: return new TreeItemNbtTagLong(parent);
        case NBTTAG_FLOAT: return new TreeItemNbtTagFloat(parent);
        case NBTTAG_DOUBLE: return new TreeItemNbtTagDouble(parent);
        case NBTTAG_BYTE_ARRAY: return new TreeItemNbtTagByteArray(parent);
        case NBTTAG_STRING: return new TreeItemNbtTagString(parent);
        case NBTTAG_LIST: return new TreeItemNbtTagList(parent);
        case NBTTAG_COMPOUND: return new TreeItemNbtTagCompound(parent);
        case NBTTAG_INT_ARRAY: return new TreeItemNbtTagIntArray(parent);
        case NBTTAG_LONG_ARRAY: return new TreeItemNbtTagLongArray(parent);
    }
    return nullptr;
}

bool readNbtFromData(TreeItem* parent, QByteArray data)
{
    quint8 type;
    TreeItemNbtTag* tag;
    QDataStream stream(data);

    stream >> type;
    if(type == NBTTAG_COMPOUND)
    {
        QString s = readStringUTF8(stream);

        if(!s.isEmpty())
        {
            tag = createItemTag(parent, type);
            tag->name = s;
            tag->readNbt(stream);
        }
        else
        {
            stream >> type;
            while(0 != type)
            {
                s = readStringUTF8(stream);
                tag = createItemTag(parent, type);
                if(tag == nullptr)
                {
                    // TODO: error
                    break;
                }
                else
                {
                    tag->name = s;
                    tag->readNbt(stream);
                }
                stream >> type;
            }
        }
        return true;
    }
    return false;
}
