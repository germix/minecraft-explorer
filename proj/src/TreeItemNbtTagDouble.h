#ifndef TREEITEMNBTTAGDOUBLE_H
#define TREEITEMNBTTAGDOUBLE_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagDouble : public TreeItemNbtTag
{
public:
    double value;
public:
    TreeItemNbtTagDouble(TreeItem* parent);
    ~TreeItemNbtTagDouble();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual bool canEdit() const override;
    virtual void openEditor(TreeModel* treeModel) override;

    virtual quint8 nbtType() const override
    {
        return NBTTAG_DOUBLE;
    }
    virtual void readNbt(QDataStream& in) override;
    virtual void writeNbt(QDataStream& out) override;
};

#endif // TREEITEMNBTTAGDOUBLE_H
