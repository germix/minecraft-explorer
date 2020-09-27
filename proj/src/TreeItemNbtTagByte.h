#ifndef TREEITEMNBTTAGBYTE_H
#define TREEITEMNBTTAGBYTE_H
#include "TreeItemNbtTag.h"

class TreeItemNbtTagByte : public TreeItemNbtTag
{
    qint8 value;
public:
    TreeItemNbtTagByte(TreeItem* parent);
    ~TreeItemNbtTagByte();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};

#endif // TREEITEMNBTTAGBYTE_H
