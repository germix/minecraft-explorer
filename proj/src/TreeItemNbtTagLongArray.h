#ifndef TREEITEMNBTTAGLONGARRAY_H
#define TREEITEMNBTTAGLONGARRAY_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagLongArray : public TreeItemNbtTag
{
public:
    QVector<qint64> data;
public:
    TreeItemNbtTagLongArray(TreeItem* parent);
    ~TreeItemNbtTagLongArray();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual quint8 nbtType() const override
    {
        return NBTTAG_LONG_ARRAY;
    }
    virtual void readNbt(QDataStream& in) override;
    virtual void writeNbt(QDataStream& out) override;
};

#endif // TREEITEMNBTTAGLONGARRAY_H
