#ifndef TREEMODEL_H
#define TREEMODEL_H
#include <QAbstractItemModel>

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
    TreeItem* root;
public:
    explicit TreeModel(QObject *parent = nullptr);
    ~TreeModel();
public:
    void load(const QString& savesPath);
    TreeItem* toItem(const QModelIndex& index) const;
    QModelIndex toIndex(TreeItem* item, int column) const;
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
    void clear();
};

#endif // TREEMODEL_H
