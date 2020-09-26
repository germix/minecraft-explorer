#include "TreeModel.h"
#include "TreeItem.h"

#define MAX_COLUMNS 1

#include <QDir>
#include <QFile>
#include <QDataStream>


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
}

void TreeModel::load(const QString& savesPath)
{
    QDir dir(savesPath);

    beginResetModel();
    clear();
    root = new TreeItem(nullptr);
    foreach(QString worldName, dir.entryList(QStringList(), QDir::AllDirs))
    {
        if(worldName == "." || worldName == "..")
        {
            continue;
        }
        new TreeItemWorld(root, worldName, savesPath + '/' + worldName);
    }
    endResetModel();
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

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
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
