#include "TreeItem.h"
#include "EditNumericValueDialog.h"

TreeItemNbtTagInt::TreeItemNbtTagInt(TreeItem* parent) : TreeItemNbtTag(parent)
{
    value = 0;
}

TreeItemNbtTagInt::~TreeItemNbtTagInt()
{
}

QIcon TreeItemNbtTagInt::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-int.png");
}

QString TreeItemNbtTagInt::getLabel() const
{
    QString s = name;
    if(!name.isEmpty())
    {
        s += ": ";
    }
    return s + stringifyValue();
}

bool TreeItemNbtTagInt::canEdit() const
{
    return true;
}

void TreeItemNbtTagInt::openEditor(TreeModel* treeModel)
{
    EditNumericValueDialog(treeModel, this).exec();
}

QString TreeItemNbtTagInt::stringifyValue() const
{
    return QString::number(value);
}

void TreeItemNbtTagInt::readNbt(QDataStream& in)
{
    in >> value;
}

void TreeItemNbtTagInt::writeNbt(QDataStream& out)
{
    out << value;
}
