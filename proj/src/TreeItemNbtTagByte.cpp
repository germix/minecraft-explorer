#include "TreeItem.h"

TreeItemNbtTagByte::TreeItemNbtTagByte(TreeItem* parent) : TreeItemNbtTag(parent)
{
}

TreeItemNbtTagByte::~TreeItemNbtTagByte()
{
}

QIcon TreeItemNbtTagByte::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-byte.png");
}

QString TreeItemNbtTagByte::getLabel() const
{
    return name + ": " + QString::number(value);
}

void TreeItemNbtTagByte::read(QDataStream& in)
{
    in >> value;
}
