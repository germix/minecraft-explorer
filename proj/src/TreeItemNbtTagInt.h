#ifndef TREEITEMNBTTAGINT_H
#define TREEITEMNBTTAGINT_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagInt : public TreeItemNbtTag
{
public:
    qint32 value;
public:
    TreeItemNbtTagInt(TreeItem* parent);
    ~TreeItemNbtTagInt();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual bool canEdit() const override;
    virtual void openEditor(TreeModel* treeModel) override;

    virtual quint8 nbtType() const override
    {
        return NBTTAG_INT;
    }
    virtual void readNbt(QDataStream& in) override;
    virtual void writeNbt(QDataStream& out) override;
};

#endif // TREEITEMNBTTAGINT_H
