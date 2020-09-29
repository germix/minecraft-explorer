#include "TreeItem.h"
#include "EditBinaryValueDialog.h"

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

bool TreeItemNbtTagByteArray::canEdit() const
{
    return true;
}

void TreeItemNbtTagByteArray::openEditor(TreeModel* treeModel)
{
    EditBinaryValueDialog(treeModel, this).exec();
}

void TreeItemNbtTagByteArray::readNbt(QDataStream& in)
{
    quint32 size;

    in >> size;

    data.resize(size);
    for(quint32 i = 0; i < size; i++)
    {
        in >> data[i];
    }
}

void TreeItemNbtTagByteArray::writeNbt(QDataStream& out)
{
    quint32 size = data.size();

    out << size;

    for(quint32 i = 0; i < size; i++)
    {
        out << data[i];
    }
}
