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
    int localChunkX;
    int localChunkZ;
    RegionFile& regionFile;
    bool canFetchData;
    int compressionMethod;
public:
    TreeItemRegionChunk(TreeItemRegionFile* parent, int chunkIndexIn, int regionXIn, int regionZIn, RegionFile& regionFileIn);
public:
    virtual TreeItem* markDirty() override;
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual void fetchMore() override;
    virtual bool canFetchMore() const override;
    virtual bool canFind() const override
    {
        return true;
    }

    void saveChunk();
};

#endif // TREEITEMREGIONCHUNK_H
