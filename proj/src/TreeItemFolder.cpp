#include "TreeItem.h"

TreeItemFolder::TreeItemFolder(TreeItem* parent, const QString& folderNameIn, const QString& parentFolderPathIn)
    : TreeItem(parent)
    , canFetchData(true)
    , folderName(folderNameIn)
    , parentFolderPath(parentFolderPathIn)
{
}

QIcon TreeItemFolder::getIcon() const
{
    return QIcon(":/images/treeitem-folder.png");
}

QString TreeItemFolder::getLabel() const
{
    return folderName;
}

void TreeItemFolder::fetchMore()
{
    canFetchData = false;
    readValidFilesInFolder(this, parentFolderPath + "/" + folderName);
    sort();
}

bool TreeItemFolder::canFetchMore() const
{
    return canFetchData;
}

bool TreeItemFolder::canFindChunk() const
{
    return folderName == "region";
}
