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
    virtual void read(QDataStream& in) = 0;
};

#endif // TREEITEMNBTTAG_H
