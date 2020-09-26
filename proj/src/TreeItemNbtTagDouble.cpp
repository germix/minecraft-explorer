#include "TreeItem.h"

TreeItemNbtTagDouble::TreeItemNbtTagDouble(TreeItem* parent) : TreeItemNbtTag(parent)
{
}

TreeItemNbtTagDouble::~TreeItemNbtTagDouble()
{
}

QIcon TreeItemNbtTagDouble::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-double.png");
}

QString TreeItemNbtTagDouble::getLabel() const
{
    return name + ": " + QString::number(value);
}

void TreeItemNbtTagDouble::read(QDataStream& in)
{
    in >> value;
}
