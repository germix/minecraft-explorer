#include "TreeItem.h"
#include <QDir>

void readValidFilesInFolder(TreeItem* parent, const QString& folder)
{
    QDir dir(folder);

    foreach(QString subDirName, dir.entryList(QStringList(), QDir::Dirs))
    {
        if(subDirName == "." || subDirName == "..")
        {
            continue;
        }
        new TreeItemFolder(parent, subDirName, folder);
    }
    foreach(QFileInfo fileInfo, dir.entryList
                (QStringList()
                    << "*.dat"
                    << "*.mca"
                    << "*.mcr"
                    << "*.json"
                    << "*.dat_old"
                    << "*.dat_mcr"
                    << "*.schematic"
                , QDir::Files))
    {
        if(fileInfo.suffix() == "dat"
                || fileInfo.suffix() == "dat_old"
                || fileInfo.suffix() == "dat_mcr"
                || fileInfo.suffix() == "schematic")
        {
            new TreeItemNbtFile(parent, fileInfo.fileName(), folder);
        }
        else if(fileInfo.suffix() == "mca" || fileInfo.suffix() == "mcr")
        {
            new TreeItemRegionFile(parent, fileInfo.fileName(), folder);
        }
        else if(fileInfo.suffix() == "json")
        {
            new TreeItemJsonFile(parent, fileInfo.fileName(), folder);
        }
    }
}

TreeItemFolderWorld::TreeItemFolderWorld(TreeItem* parent, const QString& worldNameIn, const QString& parentFolderPathIn)
    : TreeItemFolder(parent, worldNameIn, parentFolderPathIn)
{
}

QIcon TreeItemFolderWorld::getIcon() const
{
    return QIcon(":/images/treeitem-world-folder.png");
}
