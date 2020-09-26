#include "TreeItem.h"

QIcon TreeItemStatObject::getIcon() const
{
    return QIcon(":/images/treeitem-stat-item.png");
}

QString TreeItemStatObject::getLabel() const
{
    return name;
}
