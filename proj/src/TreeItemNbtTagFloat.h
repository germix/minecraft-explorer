#ifndef TREEITEMNBTTAGFLOAT_H
#define TREEITEMNBTTAGFLOAT_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagFloat : public TreeItemNbtTag
{
    float value;
public:
    TreeItemNbtTagFloat(TreeItem* parent);
    ~TreeItemNbtTagFloat();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};

#endif // TREEITEMNBTTAGFLOAT_H
