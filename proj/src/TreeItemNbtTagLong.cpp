#include "TreeItem.h"
#include "EditNumericValueDialog.h"

TreeItemNbtTagLong::TreeItemNbtTagLong(TreeItem* parent) : TreeItemNbtTag(parent)
{
    value = 0;
}

TreeItemNbtTagLong::~TreeItemNbtTagLong()
{
}

QIcon TreeItemNbtTagLong::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-long.png");
}

QString TreeItemNbtTagLong::getLabel() const
{
    QString s = name;
    if(!name.isEmpty())
    {
        s += ": ";
    }
    return s + stringifyValue();
}

bool TreeItemNbtTagLong::canEdit() const
{
    return true;
}

void TreeItemNbtTagLong::openEditor(TreeModel* treeModel)
{
    EditNumericValueDialog(treeModel, this).exec();
}

QString TreeItemNbtTagLong::stringifyValue() const
{
    return QString::number(value);
}

void TreeItemNbtTagLong::readNbt(QDataStream& in)
{
    in >> value;
}

void TreeItemNbtTagLong::writeNbt(QDataStream& out)
{
    out << value;
}
