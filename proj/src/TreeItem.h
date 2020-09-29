#ifndef TREEITEM_H
#define TREEITEM_H
#include <QVector>
#include <QIcon>
#include <QString>
#include <QDataStream>

class TreeModel;

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
    QString validPasteName(const QString& name) const;
    bool hasChildrenWithName(const QString& name) const;

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

    virtual bool canEdit() const
    {
        return false;
    }
    virtual void openEditor(TreeModel* treeModel)
    {
        Q_UNUSED(treeModel);
    }

    virtual bool canFind() const
    {
        return false;
    }

    virtual int find(const QString& text, int index)
    {
        Q_UNUSED(text);
        Q_UNUSED(index);
        return -1;
    }

    virtual QString stringifyValue() const
    {
        return QString();
    }

    virtual void cutItem()
    {
    }
    virtual bool canCutItem() const
    {
        return false;
    }

    virtual void copyItem()
    {
    }
    virtual bool canCopyItem() const
    {
        return false;
    }

    virtual void pasteIntoItem()
    {
    }
    virtual bool canPasteIntoItem() const
    {
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
