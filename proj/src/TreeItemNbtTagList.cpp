#include "TreeItem.h"
#include <QMimeData>
#include <QClipboard>
#include <QApplication>

TreeItemNbtTagList::TreeItemNbtTagList(TreeItem* parent) : TreeItemNbtTag(parent)
{
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

bool TreeItemNbtTagList::canAddNbtTag(int type) const
{
    if(children.size() > 0)
    {
        return ((TreeItemNbtTag*)children[0])->nbtType() == type;
    }
    return true;
}

bool TreeItemNbtTagList::canPasteIntoItem() const
{
    const QMimeData* md = qApp->clipboard()->mimeData();

    if(md->hasFormat("minecraft/nbt-tag"))
    {
        if(children.size() == 0)
        {
            return true;
        }
        QByteArray data = md->data("minecraft/nbt-tag");

        if(data.size() > 0)
        {
            return ((TreeItemNbtTag*)children[0])->nbtType() == data.at(0);
        }
    }
    return false;
}

void TreeItemNbtTagList::readNbt(QDataStream& in)
{
    quint8 type;
    quint32 size;

    in >> type;
    in >> size;

    for(quint32 k = 0; k < size; ++k)
    {
        TreeItemNbtTag* tag = createItemTag(this, type);
        tag->readNbt(in);
    }
}

void TreeItemNbtTagList::writeNbt(QDataStream& out)
{
    quint8 type = NBTTAG_END;
    if(children.size() > 0)
    {
        type = ((TreeItemNbtTag*)children[0])->nbtType();
    }
    quint32 size = children.size();

    out << type;
    out << size;

    for(quint32 i = 0; i < size; ++i)
    {
        ((TreeItemNbtTag*)children[i])->writeNbt(out);
    }
}
