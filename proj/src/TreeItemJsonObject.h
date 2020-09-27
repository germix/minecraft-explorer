#ifndef TREEITEMJSONOBJECT_H
#define TREEITEMJSONOBJECT_H
#include "TreeItem.h"

class TreeItemJsonObject : public TreeItem
{
public:
    QString name;
public:
    TreeItemJsonObject(TreeItem* parent, const QString& nameIn)
        : TreeItem(parent)
        , name(nameIn)
    {
        sort();
    }
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

#endif // TREEITEMJSONOBJECT_H
