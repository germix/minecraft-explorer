#include "TreeItem.h"

QIcon TreeItemJsonArray::getIcon() const
{
    return QIcon(":/images/treeitem-json-array.png");
}

QString TreeItemJsonArray::getLabel() const
{
    return name;
}
