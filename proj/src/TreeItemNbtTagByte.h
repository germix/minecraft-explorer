#ifndef TREEITEMNBTTAGBYTE_H
#define TREEITEMNBTTAGBYTE_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagByte : public TreeItemNbtTag
{
public:
    qint8 value;
public:
    TreeItemNbtTagByte(TreeItem* parent);
    ~TreeItemNbtTagByte();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual bool canEdit() const override;
    virtual void openEditor(TreeModel* treeModel) override;
    virtual QString stringifyValue() const override;

    virtual quint8 nbtType() const override
    {
        return NBTTAG_BYTE;
    }
    virtual void readNbt(QDataStream& in) override;
    virtual void writeNbt(QDataStream& out) override;
};

#endif // TREEITEMNBTTAGBYTE_H
