#include "TreeItem.h"
#include "EditNumericValueDialog.h"

TreeItemNbtTagDouble::TreeItemNbtTagDouble(TreeItem* parent) : TreeItemNbtTag(parent)
{
    value = 0;
}

TreeItemNbtTagDouble::~TreeItemNbtTagDouble()
{
}

QIcon TreeItemNbtTagDouble::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-double.png");
}

QString TreeItemNbtTagDouble::getLabel() const
{
    QString s = name;
    if(!name.isEmpty())
    {
        s += ": ";
    }
    return s + stringifyValue();
}

bool TreeItemNbtTagDouble::canEdit() const
{
    return true;
}

void TreeItemNbtTagDouble::openEditor(TreeModel* treeModel)
{
    EditNumericValueDialog(treeModel, this).exec();
}

QString TreeItemNbtTagDouble::stringifyValue() const
{
    return QString::number(value, 'g', 14);
}

void TreeItemNbtTagDouble::readNbt(QDataStream& in)
{
#if 0
    in >> value;
#else
    quint64 value64;
    in >> value64;
    value = *((double*)&value64);
#endif
}

void TreeItemNbtTagDouble::writeNbt(QDataStream& out)
{
#if 0
    out >> value;
#else
    quint64 value64;
    value64 = *((quint64*)&value);
    out << value64;
#endif
}
