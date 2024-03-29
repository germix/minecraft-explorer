#ifndef TREEMODEL_H
#define TREEMODEL_H
#include <QAbstractItemModel>
#include <QSet>

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
    TreeItem* root;
    QSet<TreeItem*> dirtyItemSet;

    friend class TreeItem;
public:
    QString rootName;
    QString rootFolder;
    QString currentFolder;
public:
    explicit TreeModel(QObject *parent = nullptr);
    ~TreeModel();
public:
    bool loadFolder(const QString& folder);
    void enterFolder(const QString& folder);
    void save();
    bool isModified() const
    {
        return dirtyItemSet.size() > 0;
    }
    TreeItem* toItem(const QModelIndex& index) const;
    QModelIndex toIndex(TreeItem* item, int column = 0) const;
    QModelIndex firstIndex() const;
    void moveItemUp(const QModelIndex& index);
    void moveItemDown(const QModelIndex& index);
    void editItem(const QModelIndex& index);
    void deleteItem(const QModelIndex& index);
    void renameItem(const QModelIndex& index, const QString& name);
    void refreshItem(const QModelIndex& index);
    void addNbtTag(const QModelIndex& parent, int type, const QString& name);
    bool hasChildrenWithName(const QModelIndex& parent, const QString& name) const;
    void itemChanged(TreeItem* item);
    QModelIndex findItem(const QModelIndex& parent, int from, const QString& name, const QString& value);
    QModelIndex findChunk(const QModelIndex& parent, int chunkX, int chunkZ);
    void cutItem(const QModelIndex& index);
    void copyItem(const QModelIndex& index);
    void pasteIntoItem(const QModelIndex& parent);
    QString validPasteName(TreeItem* parent, const QString& name);
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

    bool hasChildren(const QModelIndex& parent) const override;
    void fetchMore(const QModelIndex& parent) override;
    bool canFetchMore(const QModelIndex& parent) const override;
public:
    static bool isWorldFolder(const QString& folder);
signals:
    void onModified();
private:
    void clear();
    void markDirty(TreeItem* item);
};

#endif // TREEMODEL_H
