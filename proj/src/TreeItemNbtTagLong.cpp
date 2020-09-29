#include "TreeItem.h"

TreeItemNbtTagLong::TreeItemNbtTagLong(TreeItem* parent) : TreeItemNbtTag(parent)
{
    value = 0;
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
    QString s = name;
    if(!s.isEmpty())
    {
        s += ": " + QString::number(value);
    }
    return s;
}

void TreeItemNbtTagLong::readNbt(QDataStream& in)
{
    in >> value;
}

void TreeItemNbtTagLong::writeNbt(QDataStream& out)
{
    out << value;
}
