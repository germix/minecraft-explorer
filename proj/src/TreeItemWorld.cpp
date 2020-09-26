#include "TreeItem.h"
#include <QDir>

bool loadWorld(TreeItem* parent, const QString& worldFolderPath);

void readValidFilesInFolder(TreeItem* parent, const QString& folder)
{
    QDir dir(folder);

    foreach(QString subDirName, dir.entryList(QStringList(), QDir::Dirs))
    {
        if(subDirName == "." || subDirName == "..")
        {
            continue;
        }
        if(!loadWorld(parent, folder + "/" + subDirName))
        {
            readValidFilesInFolder(new TreeItemFolder(parent, subDirName), folder + "/" + subDirName);
        }
    }
    foreach(QFileInfo fileInfo, dir.entryList
                (QStringList()
                    << "*.dat"
                    << "*.mca"
                    << "*.json"
                , QDir::Files))
    {
        if(fileInfo.suffix() == "dat")
        {
            new TreeItemNbtFile(parent, folder, fileInfo.fileName());
        }
        else if(fileInfo.suffix() == "mca")
        {
            new TreeItemRegionFile(parent, folder, fileInfo.fileName());
        }
        else if(fileInfo.suffix() == "json")
        {
            new TreeItemJsonFile(parent, folder, fileInfo.fileName());
        }
    }
}

TreeItemWorld::TreeItemWorld(TreeItem* parent, const QString& worldNameIn, const QString& worldFolderIn)
    : TreeItem(parent)
    , worldName(worldNameIn)
    , worldFolder(worldFolderIn)
{
    readValidFilesInFolder(this, worldFolderIn);
    sort();
}

QString TreeItemWorld::getLabel() const
{
    return worldName;
}
