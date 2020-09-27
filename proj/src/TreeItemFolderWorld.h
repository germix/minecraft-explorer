#ifndef TREEITEMFOLDERWORLD_H
#define TREEITEMFOLDERWORLD_H
#include "TreeItemFolder.h"

class TreeItemFolderWorld : public TreeItemFolder
{
public:
    TreeItemFolderWorld(TreeItem* parent, const QString& worldNameIn, const QString& parentFolderPathIn);
public:
    virtual QIcon getIcon() const override;
};

#endif // TREEITEMFOLDERWORLD_H
