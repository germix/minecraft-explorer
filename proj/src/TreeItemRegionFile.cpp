#include "TreeItem.h"
#include <QFile>
#include <QDataStream>

TreeItemRegionFile::TreeItemRegionFile(TreeItem* parent, const QString& fileNameIn, const QString& parentFolderPathIn)
    : TreeItem(parent)
    , canFetchData(true)
    , fileName(fileNameIn)
    , parentFolderPath(parentFolderPathIn)
{
    QStringList parts = fileName.split(".");

    regionX = parts[1].toInt();
    regionZ = parts[2].toInt();
}

QIcon TreeItemRegionFile::getIcon() const
{
    return QIcon(":/images/treeitem-region-file.png");
}

QString TreeItemRegionFile::getLabel() const
{
    return fileName;
}

void TreeItemRegionFile::fetchMore()
{
    canFetchData = false;

    if(regionFile.open(parentFolderPath + "/" + fileName))
    {
        for(int i = 0; i < RegionFile::CHUNK_COUNT; i++)
        {
            quint16 offset = regionFile.locations[i];

            if(offset != 0)
            {
                new TreeItemRegionChunk(
                            this,
                            i,
                            regionX,
                            regionZ,
                            regionFile);
            }
        }
        sort();
    }
}

bool TreeItemRegionFile::canFetchMore() const
{
    return canFetchData;
}

void TreeItemRegionFile::saveItem()
{
    for(int i = 0; i < children.size(); i++)
    {
        TreeItemRegionChunk* regionChunk = (TreeItemRegionChunk*)children[i];

        if(regionChunk->modified)
        {
            regionChunk->saveChunk();
        }
    }
}
