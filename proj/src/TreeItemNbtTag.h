#ifndef TREEITEMNBTTAG_H
#define TREEITEMNBTTAG_H
#include "TreeItem.h"

class TreeItemNbtTag : public TreeItem
{
public:
    QString name;
public:
    TreeItemNbtTag(TreeItem* parent) : TreeItem(parent)
    {
    }
public:
    virtual TreeItem* markDirty() override;
    virtual QString getName() const override;
    virtual void renameItem(const QString& newName) override
    {
        name = newName;
    }
    virtual bool canRename() const override;

    virtual bool canDelete() const
    {
        return true;
    }

    virtual bool canMoveUp() const;

    virtual bool canMoveDown() const;

    virtual quint8 nbtType() const = 0;
    virtual void readNbt(QDataStream& in) = 0;
    virtual void writeNbt(QDataStream& out) = 0;

    virtual void cutItem() override;
    virtual void copyItem() override;
    virtual void pasteIntoItem() override;

    virtual bool canCutItem() const override
    {
        return true;
    }
    virtual bool canCopyItem() const override
    {
        return true;
    }
};

#endif // TREEITEMNBTTAG_H
