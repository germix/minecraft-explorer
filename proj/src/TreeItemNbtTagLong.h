#ifndef TREEITEMNBTTAGLONG_H
#define TREEITEMNBTTAGLONG_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagLong : public TreeItemNbtTag
{
    qint64 value;
public:
    TreeItemNbtTagLong(TreeItem* parent);
    ~TreeItemNbtTagLong();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual quint8 nbtType() const override
    {
        return NBTTAG_LONG;
    }
    virtual void readNbt(QDataStream& in) override;
    virtual void writeNbt(QDataStream& out) override;
};

#endif // TREEITEMNBTTAGLONG_H
