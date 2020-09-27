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

    virtual void read(QDataStream& in) override;
};

#endif // TREEITEMNBTTAGBYTEARRAY_H
