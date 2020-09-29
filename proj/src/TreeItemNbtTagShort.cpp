#include "TreeItem.h"
#include "EditNumericValueDialog.h"

TreeItemNbtTagShort::TreeItemNbtTagShort(TreeItem* parent) : TreeItemNbtTag(parent)
{
    value = 0;
}

TreeItemNbtTagShort::~TreeItemNbtTagShort()
{
}

QIcon TreeItemNbtTagShort::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-short.png");
}

QString TreeItemNbtTagShort::getLabel() const
{
    QString s = name;
    if(!name.isEmpty())
    {
        s += ": ";
    }
    return s + stringifyValue();
}

bool TreeItemNbtTagShort::canEdit() const
{
    return true;
}

void TreeItemNbtTagShort::openEditor(TreeModel* treeModel)
{
    EditNumericValueDialog(treeModel, this).exec();
}

QString TreeItemNbtTagShort::stringifyValue() const
{
    return QString::number(value);
}

void TreeItemNbtTagShort::readNbt(QDataStream& in)
{
    in >> value;
}

void TreeItemNbtTagShort::writeNbt(QDataStream& out)
{
    out << value;
}
