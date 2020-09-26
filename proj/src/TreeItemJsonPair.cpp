#include "TreeItem.h"

TreeItemJsonPair::TreeItemJsonPair(TreeItem* parent, const QString& nameIn, const QString& valueIn)
    : TreeItem(parent)
    , name(nameIn)
    , value(valueIn)
{
}

QIcon TreeItemJsonPair::getIcon() const
{
    return QIcon(":/images/treeitem-json-pair.png");
}

QString TreeItemJsonPair::getLabel() const
{
    if(name.isEmpty())
        return value;
    return name + ": " + value;
}
