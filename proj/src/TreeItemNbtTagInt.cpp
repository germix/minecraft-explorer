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
    return s + QString::number(value);
}

bool TreeItemNbtTagInt::canEdit() const
{
    return true;
}

void TreeItemNbtTagInt::openEditor(TreeModel* treeModel)
{
    EditNumericValueDialog(treeModel, this).exec();
}

void TreeItemNbtTagInt::readNbt(QDataStream& in)
{
    in >> value;
}

void TreeItemNbtTagInt::writeNbt(QDataStream& out)
{
    out << value;
}
