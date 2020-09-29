#ifndef TREEITEM_H
#define TREEITEM_H
#include <QVector>
#include <QIcon>
#include <QString>
#include <QDataStream>

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

    virtual bool canDelete() const
    {
        return false;
    }

    virtual bool canMoveUp() const
    {
        return false;
    }

    virtual bool canMoveDown() const
    {
        return false;
    }

    virtual bool canAddNbtTag(int type) const
    {
        Q_UNUSED(type);
        return false;
    }
};

#include "utils.h"
#include "TreeItemFolderWorld.h"
#include "TreeItemFolder.h"
#include "TreeItemNbtFile.h"
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
