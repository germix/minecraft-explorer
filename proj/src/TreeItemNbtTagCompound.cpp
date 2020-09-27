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

void TreeItemNbtTagCompound::readNbt(QDataStream& in)
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
            tag->readNbt(in);
        }
        in >> type;
    }
}

void TreeItemNbtTagCompound::writeNbt(QDataStream& out)
{
    for(quint32 i = 0; i < children.size(); i++)
    {
        TreeItemNbtTag* childTag = (TreeItemNbtTag*)children[i];

        if(childTag->nbtType() != NBTTAG_END)
        {
            out << childTag->nbtType();
            writeStringUTF8(childTag->name, out);
            childTag->writeNbt(out);
        }
    }
    out << (quint8)NBTTAG_END;
}
