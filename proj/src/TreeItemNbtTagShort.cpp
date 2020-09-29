#include "TreeItem.h"

TreeItemNbtTagShort::TreeItemNbtTagShort(TreeItem* parent) : TreeItemNbtTag(parent)
{
    value = 0;
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
    QString s = name;
    if(!name.isEmpty())
    {
        s += ": ";
    }
    return s + QString::number(value);
}

void TreeItemNbtTagShort::readNbt(QDataStream& in)
{
    in >> value;
}

void TreeItemNbtTagShort::writeNbt(QDataStream& out)
{
    out << value;
}
