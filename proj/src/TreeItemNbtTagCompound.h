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

    virtual void read(QDataStream& in) override;
};

#endif // TREEITEMNBTTAGCOMPOUND_H
