#ifndef TREEITEM_H
#define TREEITEM_H
#include <QVector>
#include <QIcon>
#include <QString>
#include <QDataStream>

class TreeItem;
class TreeItemNbtTag;

extern QString readStringUTF8(QDataStream& in);
extern void writeStringUTF8(const QString& src, QDataStream& out);
extern TreeItemNbtTag* createItemTag(TreeItem* parent, quint8 type);
extern bool gzipCompress(QByteArray input, QByteArray &output, int level);
extern bool gzipDecompress(QByteArray input, QByteArray &output);
extern void readValidFilesInFolder(TreeItem* parent, const QString& folder);
extern bool readNbtFromData(TreeItem* parent, QByteArray data);
extern QByteArray nbtTagsToByteArray(QVector<TreeItem*>& children, int compressionMethod);

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
};

enum
{
    COMPRESSION_METHOD_UNDEFINED = -1,
    COMPRESSION_METHOD_NONE = 0,
    COMPRESSION_METHOD_GZIP = 1,
    COMPRESSION_METHOD_ZLIB = 2,
};

class TreeItem
{
public:
    TreeItem* parent;
    QVector<TreeItem*> children;
public:
    TreeItem(TreeItem* parent);
    virtual ~TreeItem();
public:
    void sort();
    void clear();

    virtual QIcon getIcon() const
    {
        return QIcon();
    }
    virtual QString getLabel() const
    {
        return QString();
    }

    virtual void fetchMore()
    {
    }
    virtual bool canFetchMore() const
    {
        return false;
    }

    virtual void saveItem()
    {
    }

    virtual TreeItem* markDirty()
    {
        return nullptr;
    }

    virtual bool canRefresh() const
    {
        return false;
    }

    virtual QString getName() const
    {
        return QString();
    }
    virtual void renameItem(const QString& newName)
    {
        Q_UNUSED(newName);
    }
    virtual bool canRename() const
    {
        return false;
    }
};

#include "TreeItemFolderWorld.h"
#include "TreeItemFolder.h"
#include "TreeItemNbtFile.h"
#include "TreeItemNbtTagEnd.h"
#include "TreeItemNbtTagByte.h"
#include "TreeItemNbtTagShort.h"
#include "TreeItemNbtTagInt.h"
#include "TreeItemNbtTagLong.h"
#include "TreeItemNbtTagFloat.h"
#include "TreeItemNbtTagDouble.h"
#include "TreeItemNbtTagByteArray.h"
#include "TreeItemNbtTagString.h"
#include "TreeItemNbtTagList.h"
#include "TreeItemNbtTagCompound.h"
#include "TreeItemNbtTagIntArray.h"
#include "TreeItemNbtTagLongArray.h"
#include "TreeItemJsonFile.h"
#include "TreeItemJsonPair.h"
#include "TreeItemJsonArray.h"
#include "TreeItemJsonObject.h"
#include "TreeItemRegionFile.h"
#include "TreeItemRegionChunk.h"

#endif // TREEITEM_H
