#include "TreeModel.h"
#include "TreeItem.h"

#define MAX_COLUMNS 1

#include <QDir>
#include <QFile>
#include <QDataStream>

bool isValidNbt(const QString& fileName)
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

bool loadWorld(TreeItem* parent, const QString& worldFolderPath)
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

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    root = nullptr;
    modified = false;
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
    modified = false;
}

void TreeModel::markDirty(TreeItem* item)
{
    TreeItem* dirtyItem;

    if(nullptr != (dirtyItem = item->markDirty()))
    {
        dirtyItemSet.insert(dirtyItem);

        modified = true;

        emit onModified();
    }
}

void TreeModel::load(const QString& worldOrSavesPath)
{
    beginResetModel();
    clear();
    root = new TreeItem(nullptr);
    if(!loadWorld(root, worldOrSavesPath))
    {
        QDir dir(worldOrSavesPath);
        QString dirName = dir.dirName();
        dir.cdUp();
        QString parentPath = dir.absolutePath();
        dir.cd(dirName);
        new TreeItemFolder(root, dirName, parentPath);
    }
    root->sort();
    endResetModel();
}

void TreeModel::save()
{
    foreach(TreeItem* dirtyItem, dirtyItemSet)
    {
        dirtyItem->saveItem();
    }
    dirtyItemSet.clear();

    modified = false;
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

    beginRemoveRows(index.parent(), 0, item->children.size());
    item->clear();
    endRemoveRows();

    item->fetchMore();
    beginInsertRows(index.parent(), 0, item->children.size());
    endInsertRows();
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
    TreeItem* parentItem = toItem(parent);

    for(int i = 0; i < parentItem->children.size(); i++)
    {
        if(parentItem->children[i]->getName() == name)
        {
            return true;
        }
    }
    return false;
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
            return item->getLabel();
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
