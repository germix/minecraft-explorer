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
            new TreeItemFolder(parent, folder, subDirName);
        }
    }
    foreach(QFileInfo fileInfo, dir.entryList
                (QStringList()
                    << "*.dat"
                    << "*.mca"
                    << "*.json"
                    << "*.dat_old"
                    << "*.dat_mcr"
                , QDir::Files))
    {
        if(fileInfo.suffix() == "dat"
                || fileInfo.suffix() == "dat_old"
                || fileInfo.suffix() == "dat_mcr")
        {
            new TreeItemNbtFile(parent, fileInfo.fileName(), folder);
        }
        else if(fileInfo.suffix() == "mca")
        {
            new TreeItemRegionFile(parent, fileInfo.fileName(), folder);
        }
        else if(fileInfo.suffix() == "json")
        {
            new TreeItemJsonFile(parent, fileInfo.fileName(), folder);
        }
    }
}

TreeItemWorld::TreeItemWorld(TreeItem* parent, const QString& worldNameIn, const QString& parentFolderPathIn)
    : TreeItem(parent)
    , canFetchData(true)
    , worldName(worldNameIn)
    , parentFolderPath(parentFolderPathIn)
{
}

QIcon TreeItemWorld::getIcon() const
{
    return QIcon(":/images/treeitem-world-folder.png");
}

QString TreeItemWorld::getLabel() const
{
    return worldName;
}

void TreeItemWorld::fetchMore()
{
    canFetchData = false;
    readValidFilesInFolder(this, parentFolderPath + "/" + worldName);
    sort();
}

bool TreeItemWorld::canFetchMore() const
{
    return canFetchData;
}

