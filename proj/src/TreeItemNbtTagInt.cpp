#include "TreeItem.h"

TreeItemNbtTagInt::TreeItemNbtTagInt(TreeItem* parent) : TreeItemNbtTag(parent)
{
}

TreeItemNbtTagInt::~TreeItemNbtTagInt()
{
}

QIcon TreeItemNbtTagInt::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-int.png");
}

QString TreeItemNbtTagInt::getLabel() const
{
    return name + ": " + QString::number(value);
}

void TreeItemNbtTagInt::read(QDataStream& in)
{
    in >> value;
}
