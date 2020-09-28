#ifndef REGIONFILE_H
#define REGIONFILE_H
#include <QFile>

class RegionFile
{
public:
    enum
    {
        CHUNK_COUNT = 1024, // 32x32
    };
    QFile file;
    quint32 locations[CHUNK_COUNT];
    quint32 timestamps[CHUNK_COUNT];
public:
    RegionFile();
    ~RegionFile();
public:
    bool open(const QString& fileName);
};

#endif // REGIONFILE_H
