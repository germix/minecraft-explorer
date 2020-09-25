#include "WorldModel.h"
#include <QDir>

WorldModel::WorldModel(QObject* parent)
: QAbstractItemModel(parent)
{
}

void WorldModel::load(const QString& savesPath)
{
    QDir dir(savesPath);

    qDebug("savesPath: %ls", savesPath.constData());

    basePath = savesPath;

    beginResetModel();
    worlds.clear();
    foreach(QString worldName, dir.entryList(QStringList(), QDir::AllDirs))
    {
        qDebug("worldName: %ls", worldName.constData());
        if(worldName == "." || worldName == "..")
        {
            continue;
        }
        worlds.append(WorldItem(worldName));
    }
    endResetModel();
}

QVariant WorldModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    return QVariant();
}

QModelIndex WorldModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    if(row >= 0 && row < worlds.size() && column == 0)
    {
        return createIndex(row, column);
    }
    return QModelIndex();
}

QModelIndex WorldModel::parent(const QModelIndex& child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

int WorldModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid())
        return worlds.size();

    return 0;
}

int WorldModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant WorldModel::data(const QModelIndex& index, int role) const
{
    if(index.isValid() && index.row() >= 0 && index.row() < worlds.size())
    {
        if(role == Qt::DisplayRole)
        {
            const WorldItem& world = worlds[index.row()];

            return world.name;
        }
    }
    return QVariant();
}
