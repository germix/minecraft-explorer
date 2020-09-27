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

    virtual void read(QDataStream& in) override;
};

#endif // TREEITEMNBTTAGLIST_H
