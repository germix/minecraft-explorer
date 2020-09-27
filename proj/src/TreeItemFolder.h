#ifndef TREEITEMFOLDER_H
#define TREEITEMFOLDER_H
#include "TreeItem.h"

class TreeItemFolder : public TreeItem
{
public:
    bool canFetchData;
    QString folderName;
    QString parentFolderPath;
public:
    TreeItemFolder(TreeItem* parent, const QString& folderName, const QString& parentFolderPath);
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual void fetchMore() override;
    virtual bool canFetchMore() const override;
};

#endif // TREEITEMFOLDER_H
