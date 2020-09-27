#ifndef TREEITEMNBTTAGBYTEARRAY_H
#define TREEITEMNBTTAGBYTEARRAY_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagByteArray : public TreeItemNbtTag
{
public:
    QVector<qint8> data;
public:
    TreeItemNbtTagByteArray(TreeItem* parent);
    ~TreeItemNbtTagByteArray();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual quint8 nbtType() const override
    {
        return NBTTAG_BYTE_ARRAY;
    }
    virtual void readNbt(QDataStream& in) override;
    virtual void writeNbt(QDataStream& out) override;
};

#endif // TREEITEMNBTTAGBYTEARRAY_H
