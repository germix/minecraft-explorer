#ifndef TREEITEMNBTTAGDOUBLE_H
#define TREEITEMNBTTAGDOUBLE_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagDouble : public TreeItemNbtTag
{
    double value;
public:
    TreeItemNbtTagDouble(TreeItem* parent);
    ~TreeItemNbtTagDouble();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};

#endif // TREEITEMNBTTAGDOUBLE_H
