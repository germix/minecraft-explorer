#ifndef TREEITEMNBTTAGFLOAT_H
#define TREEITEMNBTTAGFLOAT_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagFloat : public TreeItemNbtTag
{
public:
    float value;
public:
    TreeItemNbtTagFloat(TreeItem* parent);
    ~TreeItemNbtTagFloat();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual bool canEdit() const override;
    virtual void openEditor(TreeModel* treeModel) override;

    virtual quint8 nbtType() const override
    {
        return NBTTAG_FLOAT;
    }
    virtual void readNbt(QDataStream& in) override;
    virtual void writeNbt(QDataStream& out) override;
};

#endif // TREEITEMNBTTAGFLOAT_H
