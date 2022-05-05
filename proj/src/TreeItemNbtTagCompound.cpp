#include "TreeItem.h"
#include <QMimeData>
#include <QClipboard>
#include <QApplication>

TreeItemNbtTagCompound::TreeItemNbtTagCompound(TreeItem* parent) : TreeItemNbtTag(parent)
{
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

bool TreeItemNbtTagCompound::canPasteIntoItem() const
{
    const QMimeData* md = qApp->clipboard()->mimeData();

    return md->hasFormat("minecraft/nbt-tag");
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

    qSort(this->children.begin(), this->children.end(), [](const TreeItem* a, const TreeItem* b) -> bool
    {
        TreeItemNbtTag* aTag = (TreeItemNbtTag*)a;
        TreeItemNbtTag* bTag = (TreeItemNbtTag*)b;
        return aTag->name.toLower() < bTag->name.toLower();
    });
}

void TreeItemNbtTagCompound::writeNbt(QDataStream& out)
{
    for(int i = 0; i < children.size(); i++)
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
