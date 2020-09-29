#include "TreeItem.h"

TreeItemNbtTagByte::TreeItemNbtTagByte(TreeItem* parent) : TreeItemNbtTag(parent)
{
    value = 0;
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
    QString s = name;
    if(!name.isEmpty())
    {
        s += ": ";
    }
    return s + QString::number(value);
}

void TreeItemNbtTagByte::readNbt(QDataStream& in)
{
    in >> value;
}

void TreeItemNbtTagByte::writeNbt(QDataStream& out)
{
    out << value;
}
