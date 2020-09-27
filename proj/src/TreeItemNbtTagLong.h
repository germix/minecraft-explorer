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

    virtual void read(QDataStream& in) override;
};

#endif // TREEITEMNBTTAGLONG_H
