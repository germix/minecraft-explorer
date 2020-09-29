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
    return s + QString::number(value);
}

bool TreeItemNbtTagShort::canEdit() const
{
    return true;
}

void TreeItemNbtTagShort::openEditor(TreeModel* treeModel)
{
    EditNumericValueDialog(treeModel, this).exec();
}

void TreeItemNbtTagShort::readNbt(QDataStream& in)
{
    in >> value;
}

void TreeItemNbtTagShort::writeNbt(QDataStream& out)
{
    out << value;
}
