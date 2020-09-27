#ifndef TREEITEMJSONARRAY_H
#define TREEITEMJSONARRAY_H
#include "TreeItem.h"

class TreeItemJsonArray : public TreeItem
{
public:
    QString name;
public:
    TreeItemJsonArray(TreeItem* parent, const QString& nameIn)
        : TreeItem(parent)
        , name(nameIn)
    {
    }
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

#endif // TREEITEMJSONARRAY_H
