#ifndef TREEITEMNBTTAGEND_H
#define TREEITEMNBTTAGEND_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagEnd : public TreeItemNbtTag
{
public:
    TreeItemNbtTagEnd(TreeItem* parent) : TreeItemNbtTag(parent)
    {
    }
public:
    virtual void read(QDataStream& in) override
    {
        Q_UNUSED(in);
    }
};

#endif // TREEITEMNBTTAGEND_H
