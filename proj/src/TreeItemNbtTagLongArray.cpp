#include "TreeItem.h"

TreeItemNbtTagLongArray::TreeItemNbtTagLongArray(TreeItem* parent) : TreeItemNbtTag(parent)
{
}

TreeItemNbtTagLongArray::~TreeItemNbtTagLongArray()
{
}

QIcon TreeItemNbtTagLongArray::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-byte-array.png");
}

QString TreeItemNbtTagLongArray::getLabel() const
{
    QString s;

    if(!name.isEmpty())
    {
        s = name + ": ";
    }
    if(data.size() == 1)
        s += "1 byte";
    else
        s += QObject::tr("%1 bytes").arg(data.size());
    return s;
}

void TreeItemNbtTagLongArray::readNbt(QDataStream& in)
{
    quint32 size;

    in >> size;

    data.resize(size);
    for(quint32 i = 0; i < size; i++)
    {
        in >> data[i];
    }
}

void TreeItemNbtTagLongArray::writeNbt(QDataStream& out)
{
    quint32 size = data.size();

    out << size;

    for(quint32 i = 0; i < size; i++)
    {
        out << data[i];
    }
}