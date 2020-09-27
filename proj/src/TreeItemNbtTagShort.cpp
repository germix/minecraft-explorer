#include "TreeItem.h"

TreeItemNbtTagShort::TreeItemNbtTagShort(TreeItem* parent) : TreeItemNbtTag(parent)
{
}

TreeItemNbtTagShort::~TreeItemNbtTagShort()
{
}

QIcon TreeItemNbtTagShort::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-short.png");
}

QString TreeItemNbtTagShort::getLabel() const
{
    return name + ": " + QString::number(value);
}

void TreeItemNbtTagShort::readNbt(QDataStream& in)
{
    in >> value;
}

void TreeItemNbtTagShort::writeNbt(QDataStream& out)
{
    out << value;
}
