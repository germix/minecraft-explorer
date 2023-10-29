#include "TreeModel.h"
#include "TreeItem.h"

#define MAX_COLUMNS 1

#include <QDir>
#include <QFile>
#include <QDataStream>

static bool isValidNbt(const QString& fileName)
{
    QFile file(fileName);
    if(file.open(QFile::ReadOnly) && file.size() > 2)
    {
        QByteArray data;
        data = file.read(2);

        quint8 ch1 = (quint8)data[0];
        quint8 ch2 = (quint8)data[1];
        quint8 compressionMethod = (ch1 == 0x1f && ch2 == 0x8b)
                    ? COMPRESSION_METHOD_GZIP
                    : COMPRESSION_METHOD_NONE;

        file.seek(0);
        if(compressionMethod == COMPRESSION_METHOD_NONE)
        {
            data = file.readAll();
        }
        else
        {
            data = decompressGZIP(file.readAll());
        }
        QDataStream stream(data);
        quint8 tagType;

        stream >> tagType;
        if(tagType == NBTTAG_COMPOUND)
        {
            return true;
        }
    }
    return false;
}

static bool loadWorld(TreeItem* parent, const QString& worldFolderPath)
{
    QFileInfo levelDat(worldFolderPath + "/level.dat");

    if(levelDat.exists() && isValidNbt(levelDat.absoluteFilePath()))
    {
        QDir dir(worldFolderPath);
        QString worldName = dir.dirName();
        dir.cdUp();
        QString parentPath = dir.absolutePath();
        dir.cd(worldName);

        new TreeItemFolderWorld(parent, worldName, parentPath);
        return true;
    }
    return false;
}

static void clearDirtyItem(QSet<TreeItem*>& dirtyItemSet, TreeItem* item)
{
    if(dirtyItemSet.contains(item))
    {
        dirtyItemSet.remove(item);
    }
    for(int i = 0; i < item->children.size(); i++)
    {
        clearDirtyItem(dirtyItemSet, item->children[i]);
    }
}

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    root = nullptr;
}

TreeModel::~TreeModel()
{
    clear();
}

void TreeModel::clear()
{
    if(root != nullptr)
    {
        delete root;
    }
    root = nullptr;
    dirtyItemSet.clear();
}

void TreeModel::markDirty(TreeItem* item)
{
    TreeItem* dirtyItem;

    if(nullptr != (dirtyItem = item->markDirty()))
    {
        dirtyItemSet.insert(dirtyItem);

        emit onModified();
    }
}

bool TreeModel::loadFolder(const QString& folder)
{
    enterFolder(folder);
    rootName = QFileInfo(folder).fileName();
    rootFolder = folder;

    return isWorldFolder(folder);
}

void TreeModel::enterFolder(const QString& folder)
{
    beginResetModel();
    clear();
    root = new TreeItem(nullptr);
    if(!loadWorld(root, folder))
    {
        QDir dir(folder);
        QString dirName = dir.dirName();
        dir.cdUp();
        QString parentPath = dir.absolutePath();
        dir.cd(dirName);
        new TreeItemFolder(root, dirName, parentPath);
    }
    root->sort();
    endResetModel();

    currentFolder = folder;

    emit onModified();
}

void TreeModel::save()
{
    foreach(TreeItem* dirtyItem, dirtyItemSet)
    {
        dirtyItem->saveItem();
    }
    dirtyItemSet.clear();

    emit onModified();
}

TreeItem* TreeModel::toItem(const QModelIndex& index) const
{
    if(!index.isValid())
        return 0;
    return static_cast<TreeItem*>(index.internalPointer());
}

QModelIndex TreeModel::toIndex(TreeItem* item, int column) const
{
    if(!item || (item == root))
        return QModelIndex();
    TreeItem* parent = item->parent;
    if(!parent)
        parent = root;
    int row = parent->children.lastIndexOf(item);
    if(row != -1)
        return createIndex(row, column, item);
    return QModelIndex();
}

QModelIndex TreeModel::firstIndex() const
{
    if(root != nullptr && root->children.size() > 0)
        return toIndex(root->children[0]);
    return QModelIndex();
}

void TreeModel::moveItemUp(const QModelIndex& index)
{
    int count = 1;
    int sourceRow = index.row();
    int destinationRow = index.row()-1;
    QModelIndex sourceParent = index.parent();
    QModelIndex destinationParent = index.parent();

    beginMoveRows(sourceParent, sourceRow, sourceRow+count-1, destinationParent, destinationRow);
    TreeItem* item = toItem(index);
    TreeItem* parent = item->parent;
    TreeItem* tmp = parent->children[sourceRow];
    parent->children[sourceRow] = parent->children[destinationRow];
    parent->children[destinationRow] = tmp;
    endMoveRows();

    markDirty(parent);
}

void TreeModel::moveItemDown(const QModelIndex& index)
{
    int count = 1;
    int sourceRow = index.row()+1;
    int destinationRow = index.row();
    QModelIndex sourceParent = index.parent();
    QModelIndex destinationParent = index.parent();

    beginMoveRows(sourceParent, sourceRow, sourceRow+count-1, destinationParent, destinationRow);
    TreeItem* item = toItem(index);
    TreeItem* parent = item->parent;
    TreeItem* tmp = parent->children[sourceRow];
    parent->children[sourceRow] = parent->children[destinationRow];
    parent->children[destinationRow] = tmp;
    endMoveRows();

    markDirty(parent);
}

void TreeModel::editItem(const QModelIndex& index)
{
    toItem(index)->openEditor(this);
}

void TreeModel::deleteItem(const QModelIndex& index)
{
    TreeItem* item = toItem(index);
    TreeItem* parent = item->parent;

    beginRemoveRows(index.parent(), index.row(), index.row());
    delete item;
    endRemoveRows();

    markDirty(parent);
}

void TreeModel::renameItem(const QModelIndex& index, const QString& name)
{
    TreeItem* item = toItem(index);

    item->renameItem(name);

    dataChanged(index, index);

    markDirty(item);
}

void TreeModel::refreshItem(const QModelIndex& index)
{
    TreeItem* item = toItem(index);

    clearDirtyItem(dirtyItemSet, item);

    if(item->children.size() > 0)
    {
        beginRemoveRows(index, 0, item->children.size() - 1);
        item->clear();
        endRemoveRows();
    }

    emit onModified();

    item->fetchMore();
}

void TreeModel::addNbtTag(const QModelIndex& parent, int type, const QString& name)
{
    TreeItem* parentItem = toItem(parent);
    int pos = parentItem->children.size();

    beginInsertRows(parent, pos, pos);
    createItemTag(parentItem, type)->name = name;
    endInsertRows();

    markDirty(parentItem);
}

bool TreeModel::hasChildrenWithName(const QModelIndex& parent, const QString& name) const
{
    return toItem(parent)->hasChildrenWithName(name);
}

void TreeModel::itemChanged(TreeItem* item)
{
    QModelIndex index = toIndex(item);

    markDirty(item);

    emit dataChanged(index, index);
}

QModelIndex TreeModel::findItem(const QModelIndex& parent, int from, const QString& name, const QString& value)
{
    TreeItem* parentItem = toItem(parent);

    for(int i = from; i < parentItem->children.size(); i++)
    {
        QString childName = parentItem->children[i]->getName();
        QString childValue = parentItem->children[i]->stringifyValue();

        if(!name.isNull() && !childName.isNull())
        {
            if(childName.contains(name, Qt::CaseInsensitive))
            {
                return toIndex(parentItem->children[i]);
            }
        }
        if(!value.isNull() && !childValue.isNull())
        {
            if(childValue.contains(value, Qt::CaseInsensitive))
            {
                return toIndex(parentItem->children[i]);
            }
        }

    }
    return QModelIndex();
}

QModelIndex TreeModel::findChunk(const QModelIndex& parent, int chunkX, int chunkZ)
{
    TreeItem* parentItem = toItem(parent);
    QString regionFileNameBase = "r." + QString::number(chunkX >> 5) + "." + QString::number(chunkZ >> 5);
    TreeItemRegionFile* foundRegionFileItem = nullptr;

    if(dynamic_cast<TreeItemRegionFile*>(parentItem))
    {
        foundRegionFileItem = (TreeItemRegionFile*)parentItem;
    }
    else
    {
        for(int i = 0; i < parentItem->children.size() && !foundRegionFileItem; i++)
        {
            TreeItemRegionFile* regionFileItem = dynamic_cast<TreeItemRegionFile*>(parentItem->children[i]);

            if(regionFileItem != nullptr && regionFileItem->fileName == regionFileNameBase + ".mca")
            {
                foundRegionFileItem = regionFileItem;
            }
        }
        for(int i = 0; i < parentItem->children.size() && !foundRegionFileItem; i++)
        {
            TreeItemRegionFile* regionFileItem = dynamic_cast<TreeItemRegionFile*>(parentItem->children[i]);

            if(regionFileItem != nullptr && regionFileItem->fileName == regionFileNameBase + ".mcr")
            {
                foundRegionFileItem = regionFileItem;
            }
        }
    }

    if(foundRegionFileItem != nullptr)
    {
        if(foundRegionFileItem->canFetchMore())
        {
            foundRegionFileItem->fetchMore();
        }
        for(int i = 0; i < foundRegionFileItem->children.size(); i++)
        {
            TreeItemRegionChunk* regionChunkItem = dynamic_cast<TreeItemRegionChunk*>(foundRegionFileItem->children[i]);

            if(regionChunkItem != nullptr)
            {
                if(chunkX == regionChunkItem->chunkX && chunkZ == regionChunkItem->chunkZ)
                {
                    return toIndex(regionChunkItem);
                }
            }
        }
    }

    return QModelIndex();
}

void TreeModel::cutItem(const QModelIndex& index)
{
    TreeItem* item = toItem(index);
    TreeItem* parentItem = item->parent;

    if(item->canCutItem())
    {
        beginRemoveRows(index.parent(), index.row(), index.row());
        item->cutItem();
        endRemoveRows();

        markDirty(parentItem);
    }
}

void TreeModel::copyItem(const QModelIndex& index)
{
    TreeItem* item = toItem(index);

    if(item->canCopyItem())
    {
        item->copyItem();
    }
}

void TreeModel::pasteIntoItem(const QModelIndex& parent)
{
    TreeItem* parentItem = toItem(parent);
    int pos = parentItem->children.size();

    beginInsertRows(parent, pos, pos);
    parentItem->pasteIntoItem();
    endInsertRows();

    markDirty(parentItem);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if(row < 0 || column < 0 || column >= MAX_COLUMNS)
        return QModelIndex();
    TreeItem* parentItem = parent.isValid() ? toItem(parent) : root;
    if(parentItem && row < parentItem->children.count())
    {
        TreeItem* item = parentItem->children.at(row);
        if(item)
            return createIndex(row, column, item);
        return QModelIndex();
    }
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex& child) const
{
    if(!child.isValid())
        return QModelIndex();
    TreeItem* item = static_cast<TreeItem*>(child.internalPointer());
    if(!item || item == root)
        return QModelIndex();
    return toIndex(item->parent, 0);
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
    if(!root)
        return 0;
    if(!parent.isValid())
        return root->children.count();
    TreeItem* parentItem = toItem(parent);
    if(parentItem)
        return parentItem->children.count();
    return 0;
}

int TreeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return MAX_COLUMNS;
}

QVariant TreeModel::data(const QModelIndex& index, int role) const
{
    if(index.isValid())
    {
        TreeItem* item = toItem(index);

        if(role == Qt::DisplayRole)
        {
            QString s = item->getLabel();
            if(dirtyItemSet.contains(item))
            {
                s = "* " + s;
            }
            return s;
        }
        else if(role == Qt::DecorationRole)
        {
            return item->getIcon();
        }
    }
    return QVariant();
}

bool TreeModel::hasChildren(const QModelIndex& parent) const
{
    if(!root)
        return 0;
    if(!parent.isValid())
        return (root->children.count() > 0);
    TreeItem* item = toItem(parent);
    if(!item)
        return false;
    return (item->children.count() > 0 || item->canFetchMore());
}

void TreeModel::fetchMore(const QModelIndex& parent)
{
    TreeItem* parentItem;

    if(nullptr != (parentItem = toItem(parent)))
    {
        parentItem->fetchMore();
        beginInsertRows(parent, 0, parentItem->children.size());
        endInsertRows();
    }
}

bool TreeModel::canFetchMore(const QModelIndex& parent) const
{
    TreeItem* parentItem;

    if(nullptr != (parentItem = toItem(parent)))
    {
        return parentItem->canFetchMore();
    }
    return true;
}

bool TreeModel::isWorldFolder(const QString& folder)
{
    QFileInfo levelDat(folder + "/level.dat");

    return (levelDat.exists() && isValidNbt(levelDat.absoluteFilePath()));
}
