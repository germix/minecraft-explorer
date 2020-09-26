#include "TreeItem.h"

#include <QDir>
#include <QFile>
#include <QDataStream>

#include <zlib.h>


#define GZIP_WINDOWS_BIT 15 + 16
#define GZIP_CHUNK_SIZE 32 * 1024

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

enum NBTTAG
{
    NBTTAG_END = 0,
    NBTTAG_BYTE = 1,
    NBTTAG_SHORT = 2,
    NBTTAG_INT = 3,
    NBTTAG_LONG = 4,
    NBTTAG_FLOAT = 5,
    NBTTAG_DOUBLE = 6,
    NBTTAG_BYTE_ARRAY = 7,
    NBTTAG_STRING = 8,
    NBTTAG_LIST = 9,
    NBTTAG_COMPOUND = 10,
    NBTTAG_INT_ARRAY = 11,
};
static wchar_t utf8_to_unicode(QDataStream& in)
{
    // Reference: https://stackoverflow.com/questions/18534494/convert-from-utf-8-to-unicode-c
    quint8 t;
    quint8 ch;

    int charcode = 0;
    in >> t;
    if(t < 128)
    {
        return t;
    }
    int high_bit_mask = (1 << 6) -1;
    int high_bit_shift = 0;
    int total_bits = 0;
    const int other_bits = 6;
    while((t & 0xC0) == 0xC0)
    {
        in >> ch;

        t <<= 1;
        t &= 0xff;
        total_bits += 6;
        high_bit_mask >>= 1;
        high_bit_shift++;
        charcode <<= other_bits;
        charcode |= ch & ((1 << other_bits)-1);
    }
    charcode |= ((t >> high_bit_shift) & high_bit_mask) << total_bits;
    return charcode;
}
QString readStringUTF8(QDataStream& in)
{
    quint16 utflen;
    QString utfstr;

    in >> utflen;

    while(utflen > 0)
    {
        utflen--;
        utfstr += (wchar_t)utf8_to_unicode(in);
    }
    return utfstr;
}

TreeItemNbtTag* createItemTag(TreeItem* parent, quint8 type)
{
    switch(type)
    {
        case NBTTAG_END: return new TreeItemNbtTagEnd(parent);
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
    }
    return nullptr;
}

TreeItem::TreeItem(TreeItem* ___parent)
    : parent(___parent)
{
    if(parent)
        parent->children.append(this);
}

TreeItem::~TreeItem()
{
    //
    // Remover del padre
    //
    if(parent)
    {
        parent->children.removeOne(this);
    }
    //
    // Eliminar hijos
    //
    for(int i = 0; i < children.count(); i++)
    {
        TreeItem* child = children.at(i);
        // ...
        child->parent = nullptr;
        // ...
        delete child;
    }
    children.clear();
}

QIcon TreeItem::getIcon() const
{
    return QIcon();
}

QString TreeItem::getLabel() const
{
    return QString();
}

TreeItemFolder::TreeItemFolder(TreeItem* parent, const QString& folderNameIn)
    : TreeItem(parent)
    , folderName(folderNameIn)
{
}
