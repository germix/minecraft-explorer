#ifndef TREEITEMNBTTAGLIST_H
#define TREEITEMNBTTAGLIST_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagList : public TreeItemNbtTag
{
public:
    TreeItemNbtTagList(TreeItem* parent);
    ~TreeItemNbtTagList();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual bool canAddNbtTag(int type) const override;
    virtual bool canFind() const override
    {
        return true;
    }
    virtual bool canPasteIntoItem() const override;

    virtual quint8 nbtType() const override
    {
        return NBTTAG_LIST;
    }
    virtual void readNbt(QDataStream& in) override;
    virtual void writeNbt(QDataStream& out) override;
};

#endif // TREEITEMNBTTAGLIST_H
