#include "TreeItem.h"

TreeItemNbtTagInt::TreeItemNbtTagInt(TreeItem* parent) : TreeItemNbtTag(parent)
{
    value = 0;
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
    QString s = name;
    if(!name.isEmpty())
    {
        s += ": ";
    }
    return s + QString::number(value);
}

void TreeItemNbtTagInt::readNbt(QDataStream& in)
{
    in >> value;
}

void TreeItemNbtTagInt::writeNbt(QDataStream& out)
{
    out << value;
}
