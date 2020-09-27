#ifndef TREEITEMWORLD_H
#define TREEITEMWORLD_H
#include "TreeItem.h"

class TreeItemWorld : public TreeItem
{
public:
    bool canFetchData;
    QString worldName;
    QString parentFolderPath;
public:
    TreeItemWorld(TreeItem* parent, const QString& worldNameIn, const QString& parentFolderPathIn);
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual void fetchMore() override;
    virtual bool canFetchMore() const override;
};

#endif // TREEITEMWORLD_H
