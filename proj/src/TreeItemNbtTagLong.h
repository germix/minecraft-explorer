#ifndef TREEITEMNBTTAGLONG_H
#define TREEITEMNBTTAGLONG_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagLong : public TreeItemNbtTag
{
public:
    qint64 value;
public:
    TreeItemNbtTagLong(TreeItem* parent);
    ~TreeItemNbtTagLong();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual bool canEdit() const override;
    virtual void openEditor(TreeModel* treeModel) override;
    virtual QString stringifyValue() const override;

    virtual quint8 nbtType() const override
    {
        return NBTTAG_LONG;
    }
    virtual void readNbt(QDataStream& in) override;
    virtual void writeNbt(QDataStream& out) override;
};

#endif // TREEITEMNBTTAGLONG_H
