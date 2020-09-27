#ifndef TREEITEMNBTTAGINTARRAY_H
#define TREEITEMNBTTAGINTARRAY_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagIntArray : public TreeItemNbtTag
{
public:
    QVector<qint32> data;
public:
    TreeItemNbtTagIntArray(TreeItem* parent);
    ~TreeItemNbtTagIntArray();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual quint8 nbtType() const override
    {
        return NBTTAG_INT_ARRAY;
    }
    virtual void readNbt(QDataStream& in) override;
    virtual void writeNbt(QDataStream& out) override;
};

#endif // TREEITEMNBTTAGINTARRAY_H
