#ifndef TREEITEMJSONPAIR_H
#define TREEITEMJSONPAIR_H
#include "TreeItem.h"

class TreeItemJsonPair : public TreeItem
{
public:
    QString name;
    QString value;
public:
    TreeItemJsonPair(TreeItem* parent, const QString& nameIn, const QString& valueIn);
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

#endif // TREEITEMJSONPAIR_H
