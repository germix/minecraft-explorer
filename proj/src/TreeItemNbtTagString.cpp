#include "TreeItem.h"

TreeItemNbtTagString::TreeItemNbtTagString(TreeItem* parent) : TreeItemNbtTag(parent)
{
}

TreeItemNbtTagString::~TreeItemNbtTagString()
{
}

QIcon TreeItemNbtTagString::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-string.png");
}

QString TreeItemNbtTagString::getLabel() const
{
    QString s = name;
    if(!name.isEmpty())
    {
        s += ": ";
    }
    return s + value;
}

void TreeItemNbtTagString::readNbt(QDataStream& in)
{
    value = readStringUTF8(in);
}

void TreeItemNbtTagString::writeNbt(QDataStream& out)
{
    writeStringUTF8(value, out);
}
