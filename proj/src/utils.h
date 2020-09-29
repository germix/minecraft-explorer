#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <QByteArray>
#include <QDataStream>

class TreeItem;
class TreeItemNbtTag;

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
    NBTTAG_LONG_ARRAY = 12,
    MAX_NBTTAGS,
};

enum
{
    COMPRESSION_METHOD_UNDEFINED = -1,
    COMPRESSION_METHOD_NONE = 0,
    COMPRESSION_METHOD_GZIP = 1,
    COMPRESSION_METHOD_ZLIB = 2,
};

QByteArray compressGZIP(QByteArray data, int level);
QByteArray decompressGZIP(QByteArray data);
QByteArray compressZLIB(QByteArray data);
QByteArray decompressZLIB(QByteArray data);

extern QString readStringUTF8(QDataStream& in);
extern void writeStringUTF8(const QString& src, QDataStream& out);

extern TreeItemNbtTag* createItemTag(TreeItem* parent, quint8 type);
extern void readValidFilesInFolder(TreeItem* parent, const QString& folder);
extern bool readNbtFromData(TreeItem* parent, QByteArray data);
extern QByteArray nbtTagsToByteArray(QVector<TreeItem*>& children, int compressionMethod);

void parseNbtTagFromClipboard(TreeItem* parent);

#endif // UTILS_H
