#include "TreeItem.h"

QIcon TreeItemJsonObject::getIcon() const
{
    return QIcon(":/images/treeitem-json-object.png");
}

QString TreeItemJsonObject::getLabel() const
{
    return name;
}
