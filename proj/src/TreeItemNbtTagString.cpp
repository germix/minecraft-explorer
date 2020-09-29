#include "TreeItem.h"
#include "EditStringValueDialog.h"

TreeItemNbtTagString::TreeItemNbtTagString(TreeItem* parent) : TreeItemNbtTag(parent)
{
}

TreeItemNbtTagString::~TreeItemNbtTagString()
{
}

QIcon TreeItemNbtTagString::getIcon() const
{
    return QIcon(":/images/treeitem-nbt-tag-string.png");
}

QString TreeItemNbtTagString::getLabel() const
{
    QString s = name;
    if(!name.isEmpty())
    {
        s += ": ";
    }
    return s + value;
}

bool TreeItemNbtTagString::canEdit() const
{
    return true;
}

void TreeItemNbtTagString::openEditor(TreeModel* treeModel)
{
    EditStringValueDialog(treeModel, this).exec();
}

QString TreeItemNbtTagString::stringifyValue() const
{
    return value;
}

void TreeItemNbtTagString::readNbt(QDataStream& in)
{
    value = readStringUTF8(in);
}

void TreeItemNbtTagString::writeNbt(QDataStream& out)
{
    writeStringUTF8(value, out);
}
