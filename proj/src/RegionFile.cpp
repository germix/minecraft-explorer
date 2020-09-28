#include "RegionFile.h"
#include <QDataStream>

RegionFile::RegionFile()
{
    memset(locations, 0, sizeof(locations));
    memset(timestamps, 0, sizeof(timestamps));
}

RegionFile::~RegionFile()
{
}

bool RegionFile::open(const QString& fileName)
{
    file.close();
    file.setFileName(fileName);
    if(file.open(QFile::ReadWrite))
    {
        if(file.size() < 8192)
        {
            file.close();
            return false;
        }
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

        return true;
    }
    return false;
}
