#ifndef TREEITEMNBTTAGSHORT_H
#define TREEITEMNBTTAGSHORT_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagShort : public TreeItemNbtTag
{
public:
    qint16 value;
public:
    TreeItemNbtTagShort(TreeItem* parent);
    ~TreeItemNbtTagShort();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual bool canEdit() const override;
    virtual void openEditor(TreeModel* treeModel) override;
    virtual QString stringifyValue() const override;

    virtual quint8 nbtType() const override
    {
        return NBTTAG_SHORT;
    }
    virtual void readNbt(QDataStream& in) override;
    virtual void writeNbt(QDataStream& out) override;
};

#endif // TREEITEMNBTTAGSHORT_H
