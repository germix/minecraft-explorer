#ifndef TREEITEMNBTTAGCOMPOUND_H
#define TREEITEMNBTTAGCOMPOUND_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagCompound : public TreeItemNbtTag
{
public:
    TreeItemNbtTagCompound(TreeItem* parent);
    ~TreeItemNbtTagCompound();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual bool canAddNbtTag(int type) const override
    {
        Q_UNUSED(type);
        return true;
    }
    virtual bool canFind() const override
    {
        return true;
    }
    virtual bool canPasteIntoItem() const override;

    virtual quint8 nbtType() const override
    {
        return NBTTAG_COMPOUND;
    }
    virtual void readNbt(QDataStream& in) override;
    virtual void writeNbt(QDataStream& out) override;
};

#endif // TREEITEMNBTTAGCOMPOUND_H
