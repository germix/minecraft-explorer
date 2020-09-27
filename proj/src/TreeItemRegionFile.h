#ifndef TREEITEMREGIONFILE_H
#define TREEITEMREGIONFILE_H
#include "TreeItem.h"

class TreeItemRegionFile : public TreeItem
{
public:
    bool canFetchData;
    QString fileName;
    QString parentFolderPath;
public:
    TreeItemRegionFile(TreeItem* parent, const QString& fileNameIn, const QString& parentFolderPathIn);
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual void fetchMore() override;
    virtual bool canFetchMore() const override;
    virtual void saveItem() override;
};

#endif // TREEITEMREGIONFILE_H
