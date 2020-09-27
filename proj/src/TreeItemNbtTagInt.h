#ifndef TREEITEMNBTTAGINT_H
#define TREEITEMNBTTAGINT_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagInt : public TreeItemNbtTag
{
    qint32 value;
public:
    TreeItemNbtTagInt(TreeItem* parent);
    ~TreeItemNbtTagInt();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};

#endif // TREEITEMNBTTAGINT_H
