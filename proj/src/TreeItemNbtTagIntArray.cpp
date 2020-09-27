#include "TreeItem.h"

TreeItemNbtTagIntArray::TreeItemNbtTagIntArray(TreeItem* parent) : TreeItemNbtTag(parent)
{
}

TreeItemNbtTagIntArray::~TreeItemNbtTagIntArray()
{
}

QIcon TreeItemNbtTagIntArray::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-int-array.png");
}

QString TreeItemNbtTagIntArray::getLabel() const
{
    QString s;

    if(!name.isEmpty())
    {
        s = name + ": ";
    }
    if(data.size() == 1)
        s += "1 integer";
    else
        s += QObject::tr("%1 integers").arg(data.size());
    return s;
}

void TreeItemNbtTagIntArray::readNbt(QDataStream& in)
{
    quint32 size;

    in >> size;

    data.resize(size);
    for(quint32 i = 0; i < size; i++)
    {
        in >> data[i];
    }
}

void TreeItemNbtTagIntArray::writeNbt(QDataStream& out)
{
    quint32 size = data.size();

    out << size;

    for(quint32 i = 0; i < size; i++)
    {
        out << data[i];
    }
}
