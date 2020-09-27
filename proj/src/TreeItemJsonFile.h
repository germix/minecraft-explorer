#ifndef TREEITEMJSONFILE_H
#define TREEITEMJSONFILE_H
#include "TreeItem.h"

class TreeItemJsonFile : public TreeItem
{
public:
    bool canFetchData;
    QString fileName;
    QString parentFolderPath;
public:
    TreeItemJsonFile(TreeItem* parent, const QString& fileNameIn, const QString& parentFolderPathIn);
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual void fetchMore() override;
    virtual bool canFetchMore() const override;
    virtual bool canRefresh() const override
    {
        return true;
    }
};

#endif // TREEITEMJSONFILE_H
