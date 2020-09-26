#include "TreeItem.h"

TreeItemNbtTagFloat::TreeItemNbtTagFloat(TreeItem* parent) : TreeItemNbtTag(parent)
{
}

TreeItemNbtTagFloat::~TreeItemNbtTagFloat()
{
}

QIcon TreeItemNbtTagFloat::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-float.png");
}

QString TreeItemNbtTagFloat::getLabel() const
{
    return name + ": " + QString::number(value);
}

void TreeItemNbtTagFloat::read(QDataStream& in)
{
    in >> value;
}
