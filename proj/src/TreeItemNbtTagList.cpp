#include "TreeItem.h"

TreeItemNbtTagList::TreeItemNbtTagList(TreeItem* parent) : TreeItemNbtTag(parent)
{
    sort();
}

TreeItemNbtTagList::~TreeItemNbtTagList()
{
}

QIcon TreeItemNbtTagList::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-list.png");
}

QString TreeItemNbtTagList::getLabel() const
{
    QString s;

    if(!name.isEmpty())
    {
        s = name + ": ";
    }
    if(children.size() == 1)
        s += "1 entry";
    else
        s += QObject::tr("%1 entries").arg(children.size());
    return s;
}

void TreeItemNbtTagList::read(QDataStream& in)
{
    quint8 type;
    quint32 size;

    in >> type;
    in >> size;

    for(quint32 k = 0; k < size; ++k)
    {
        TreeItemNbtTag* tag = createItemTag(this, type);
        tag->read(in);
    }
}
