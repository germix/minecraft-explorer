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

    virtual void read(QDataStream& in) override;
};

#endif // TREEITEMNBTTAGINTARRAY_H
