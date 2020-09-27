#ifndef TREEITEMREGIONCHUNK_H
#define TREEITEMREGIONCHUNK_H
#include "TreeItem.h"

class QFile;
class TreeItemRegionChunk : public TreeItem
{
public:
    int chunkX;
    int chunkZ;
    quint32 location;
    quint32 timestamp;
    int compressionMethod;
public:
    TreeItemRegionChunk(TreeItemRegionFile* parent, QFile& file, int x, int z, quint32 locationIn, quint32 timestampIn);
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

#endif // TREEITEMREGIONCHUNK_H
