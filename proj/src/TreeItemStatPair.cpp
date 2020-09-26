#include "TreeItem.h"

QIcon TreeItemStatPair::getIcon() const
{
    return QIcon(":/images/treeitem-stat-item.png");
}

QString TreeItemStatPair::getLabel() const
{
    if(name.isEmpty())
        return value;
    return name + ": " + value;
}
