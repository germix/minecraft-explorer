#include "TreeItem.h"

TreeItemNbtTagCompound::TreeItemNbtTagCompound(TreeItem* parent) : TreeItemNbtTag(parent)
{
    sort();
}

TreeItemNbtTagCompound::~TreeItemNbtTagCompound()
{
}

QIcon TreeItemNbtTagCompound::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-compound.png");
}

QString TreeItemNbtTagCompound::getLabel() const
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

void TreeItemNbtTagCompound::read(QDataStream& in)
{
    quint8 type;

    in >> type;
    while(0 != type)
    {
        QString s = readStringUTF8(in);
        TreeItemNbtTag* tag = createItemTag(this, type);

        if(tag == nullptr)
        {
            // TODO: error
            break;
        }
        else
        {
            tag->name = s;
            tag->read(in);
        }
        in >> type;
    }
}
