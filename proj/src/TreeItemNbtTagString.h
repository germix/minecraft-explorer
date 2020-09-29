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
    virtual bool canEdit() const override;
    virtual void openEditor(TreeModel* treeModel) override;

    virtual quint8 nbtType() const override
    {
        return NBTTAG_STRING;
    }
    virtual void readNbt(QDataStream& in) override;
    virtual void writeNbt(QDataStream& out) override;
};

#endif // TREEITEMNBTTAGSTRING_H
