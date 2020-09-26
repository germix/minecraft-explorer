#include "TreeItem.h"

TreeItemNbtTagByteArray::TreeItemNbtTagByteArray(TreeItem* parent) : TreeItemNbtTag(parent)
{
}

TreeItemNbtTagByteArray::~TreeItemNbtTagByteArray()
{
}

QIcon TreeItemNbtTagByteArray::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-byte-array.png");
}

QString TreeItemNbtTagByteArray::getLabel() const
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

void TreeItemNbtTagByteArray::read(QDataStream& in)
{
    quint32 size;

    in >> size;

    data.resize(size);
    for(int i = 0; i < size; i++)
    {
        in >> data[i];
    }
}
