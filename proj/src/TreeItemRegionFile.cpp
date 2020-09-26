#include "TreeItem.h"
#include <QFile>
#include <QDataStream>

TreeItemRegionFile::TreeItemRegionFile(TreeItem* parent, const QString& folder, const QString& fileName)
    : TreeItem(parent)
{
    name = fileName;
    enum
    {
        CHUNK_COUNT = 1024,
    };
    quint32 locations[CHUNK_COUNT];
    quint32 timestamps[CHUNK_COUNT];

    memset(locations, 0, sizeof(locations));
    memset(timestamps, 0, sizeof(timestamps));

    QFile file(folder + "/" + fileName);
    if(file.open(QFile::ReadOnly))
    {
        if(file.size() < 8192)
        {
            // TODO
        }
        else
        {
            QDataStream stream(&file);

            // Read locations
            for(int i = 0; i < CHUNK_COUNT; i++)
            {
                stream >> locations[i];
            }
            // Read timestamps
            for(int i = 0; i < CHUNK_COUNT; i++)
            {
                stream >> timestamps[i];
            }

            for(int i = 0; i < CHUNK_COUNT; i++)
            {
                quint16 offset = locations[i] & 0xffff;

                if(offset != 0)
                {
                    new TreeItemRegionChunk(
                                this,
                                file,
                                i&31,
                                i/32,
                                locations[i], timestamps[i]);
                }
            }
        }
    }
}

QIcon TreeItemRegionFile::getIcon() const
{
    return QIcon(":/images/treeitem-region-file.png");
}

QString TreeItemRegionFile::getLabel() const
{
    return name;
}
