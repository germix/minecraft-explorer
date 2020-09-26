#include "TreeItem.h"
#include <QDir>

int readNbtDatFilesInFolder(TreeItem* parent, const QString& folder)
{
    QDir dir(folder);
    int count = 0;

    foreach(QString subDirName, dir.entryList(QStringList(), QDir::Dirs))
    {
        if(subDirName == "." || subDirName == "..")
        {
            continue;
        }
        count += readNbtDatFilesInFolder(new TreeItemFolder(parent, subDirName), folder + "/" + subDirName);
    }
    foreach(QString datFileName, dir.entryList(QStringList() << "*.dat", QDir::Files))
    {
        count++;
        new TreeItemNbtFile(parent, folder, datFileName);
    }
    if(count == 0)
    {
        delete parent;
    }

    return count;
}

TreeItemWorld::TreeItemWorld(TreeItem* parent, const QString& worldNameIn, const QString& worldFolderIn)
    : TreeItem(parent)
    , worldName(worldNameIn)
    , worldFolder(worldFolderIn)
{
    readNbtDatFilesInFolder(this, worldFolderIn);

    {
        QDir statsDir(worldFolder + "/stats");
        QString statsFolder = statsDir.absolutePath();
        TreeItemFolder* statsFolderItem = new TreeItemFolder(this, "stats");

        foreach(QString statFileName, statsDir.entryList(QStringList() << "*.json", QDir::Files))
        {
            new TreeItemStatFile(statsFolderItem, statsFolder, statFileName);
        }
    }
    {
        QDir regionDir(worldFolder + "/region");
        QString regionFolder = regionDir.absolutePath();
        TreeItemFolder* regionFolderItem = new TreeItemFolder(this, "region");

        foreach(QString regionFileName, regionDir.entryList(QStringList() << "*.mca", QDir::Files))
        {
            new TreeItemRegionFile(regionFolderItem, regionFolder, regionFileName);
        }
    }
}

QString TreeItemWorld::getLabel() const
{
    return worldName;
}
