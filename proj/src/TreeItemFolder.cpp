#include "TreeItem.h"

TreeItemFolder::TreeItemFolder(TreeItem* parent, const QString& parentFolderPathIn, const QString& folderNameIn)
    : TreeItem(parent)
    , canFetchData(true)
    , folderName(folderNameIn)
    , parentFolderPath(parentFolderPathIn)
{
    canFetchData = true;
    sort();
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
}

bool TreeItemFolder::canFetchMore() const
{
    return canFetchData;
}
