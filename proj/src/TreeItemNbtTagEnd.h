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
    virtual quint8 nbtType() const override
    {
        return NBTTAG_END;
    }
    virtual void readNbt(QDataStream& in)
    {
        Q_UNUSED(in);
    }
    virtual void writeNbt(QDataStream& out)
    {
        Q_UNUSED(out);
    }
};

#endif // TREEITEMNBTTAGEND_H
