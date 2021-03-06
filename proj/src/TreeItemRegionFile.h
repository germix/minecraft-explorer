#ifndef TREEITEMREGIONFILE_H
#define TREEITEMREGIONFILE_H
#include "TreeItem.h"
#include "RegionFile.h"

class TreeItemRegionFile : public TreeItem
{
public:
    bool canFetchData;
    QString fileName;
    QString parentFolderPath;
    int regionX;
    int regionZ;
    RegionFile regionFile;
public:
    TreeItemRegionFile(TreeItem* parent, const QString& fileNameIn, const QString& parentFolderPathIn);
    ~TreeItemRegionFile();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual void fetchMore() override;
    virtual bool canFetchMore() const override;
    virtual void saveItem() override;
    virtual bool canRefresh() const override
    {
        return true;
    }
    virtual bool canFindChunk() const override
    {
        return true;
    }
};

#endif // TREEITEMREGIONFILE_H
