#include "TreeItem.h"

QIcon TreeItemStatArray::getIcon() const
{
    return QIcon(":/images/treeitem-stat-item.png");
}

QString TreeItemStatArray::getLabel() const
{
    return name;
}
