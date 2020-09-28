#ifndef TREEITEMREGIONCHUNK_H
#define TREEITEMREGIONCHUNK_H
#include "TreeItem.h"

class QFile;
class TreeItemRegionChunk : public TreeItem
{
public:
    int chunkX;
    int chunkZ;
    int chunkIndex;
    RegionFile& regionFile;
    bool canFetchData;
    int compressionMethod;
    bool modified;
public:
    TreeItemRegionChunk(TreeItemRegionFile* parent, int chunkIndexIn, RegionFile& regionFileIn);
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual void fetchMore() override;
    virtual bool canFetchMore() const override;

    void saveChunk();
};

#endif // TREEITEMREGIONCHUNK_H
