#ifndef TREEITEMNBTTAGSHORT_H
#define TREEITEMNBTTAGSHORT_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagShort : public TreeItemNbtTag
{
    qint16 value;
public:
    TreeItemNbtTagShort(TreeItem* parent);
    ~TreeItemNbtTagShort();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};

#endif // TREEITEMNBTTAGSHORT_H
