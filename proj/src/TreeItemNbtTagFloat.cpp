#include "TreeItem.h"
#include "EditNumericValueDialog.h"

TreeItemNbtTagFloat::TreeItemNbtTagFloat(TreeItem* parent) : TreeItemNbtTag(parent)
{
    value = 0;
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
    QString s = name;
    if(!name.isEmpty())
    {
        s += ": ";
    }
    return s + stringifyValue();
}

bool TreeItemNbtTagFloat::canEdit() const
{
    return true;
}

void TreeItemNbtTagFloat::openEditor(TreeModel* treeModel)
{
    EditNumericValueDialog(treeModel, this).exec();
}

QString TreeItemNbtTagFloat::stringifyValue() const
{
    return QString::number(value);
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
