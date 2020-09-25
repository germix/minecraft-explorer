#ifndef WORLDMODEL_H
#define WORLDMODEL_H
#include <QAbstractItemModel>
#include <QVector>

class WorldModel : public QAbstractItemModel
{
    Q_OBJECT
    class WorldItem
    {
    public:
        QString name;
        WorldItem()
        {
        }
        WorldItem(const QString& n)
            : name(n)
        {
        }
        WorldItem(const WorldItem& o)
            : name(o.name)
        {
        }
    public:
        WorldItem& operator = (const WorldItem& o)
        {
            name = o.name;
            return *this;
        }
    };
    QString basePath;
    QVector<WorldItem> worlds;
public:
    explicit WorldModel(QObject* parent = nullptr);
public:
    void load(const QString& savesPath);
public:
    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
private:
};

#endif // WORLDMODEL_H
