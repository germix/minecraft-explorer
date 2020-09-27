#ifndef TREEITEMNBTTAGSTRING_H
#define TREEITEMNBTTAGSTRING_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagString : public TreeItemNbtTag
{
public:
    QString value;
public:
    TreeItemNbtTagString(TreeItem* parent);
    ~TreeItemNbtTagString();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};

#endif // TREEITEMNBTTAGSTRING_H
