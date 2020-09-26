#include "TreeItem.h"

TreeItemNbtTagLong::TreeItemNbtTagLong(TreeItem* parent) : TreeItemNbtTag(parent)
{
}

TreeItemNbtTagLong::~TreeItemNbtTagLong()
{
}

QIcon TreeItemNbtTagLong::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-long.png");
}

QString TreeItemNbtTagLong::getLabel() const
{
    return name + ": " + QString::number(value);
}

void TreeItemNbtTagLong::read(QDataStream& in)
{
    in >> value;
}
