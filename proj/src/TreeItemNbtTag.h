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
    virtual quint8 nbtType() const = 0;
    virtual void readNbt(QDataStream& in) = 0;
    virtual void writeNbt(QDataStream& out) = 0;
};

#endif // TREEITEMNBTTAG_H
