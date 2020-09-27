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

void TreeItemNbtTagFloat::readNbt(QDataStream& in)
{
#if 0
    in >> value;
#else
    quint32 value32;
    in >> value32;
    value = *((float*)&value32);
#endif
}

void TreeItemNbtTagFloat::writeNbt(QDataStream& out)
{
#if 0
    in >> value;
#else
    quint32 value32;
    value32 = *((quint32*)&value);
    out << value32;
#endif
}
