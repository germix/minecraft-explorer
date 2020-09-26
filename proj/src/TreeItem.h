#ifndef TREEITEM_H
#define TREEITEM_H
#include <QVector>
#include <QIcon>
#include <QString>
#include <QDataStream>

class TreeItem;
class TreeItemNbtTag;

extern QString readStringUTF8(QDataStream& in);
extern TreeItemNbtTag* createItemTag(TreeItem* parent, quint8 type);
extern bool gzipDecompress(QByteArray input, QByteArray &output);

class TreeItem
{
public:
    TreeItem* parent;
    QVector<TreeItem*> children;
public:
    TreeItem(TreeItem* parent);
    virtual ~TreeItem();
public:
    void sort();
    virtual QIcon getIcon() const;
    virtual QString getLabel() const;
};

class TreeItemWorld : public TreeItem
{
public:
    QString worldName;
    QString worldFolder;
public:
    TreeItemWorld(TreeItem* parent, const QString& worldNameIn, const QString& worldFolderIn);
public:
    virtual QIcon getIcon() const override
    {
        return QIcon(":/images/treeitem-world-folder.png");
    }
    virtual QString getLabel() const override;
};

class TreeItemFolder : public TreeItem
{
public:
    QString folderName;
public:
    TreeItemFolder(TreeItem* parent, const QString& folderName);
public:
    virtual QIcon getIcon() const override
    {
        return QIcon(":/images/treeitem-folder.png");
    }
    virtual QString getLabel() const override
    {
        return folderName;
    }
};

class TreeItemNbtFile : public TreeItem
{
public:
    QString name;
    bool isCompressed;
public:
    TreeItemNbtFile(TreeItem* parent, const QString& folder, const QString& fileName);
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class TreeItemNbtTag : public TreeItem
{
public:
    QString name;
public:
    TreeItemNbtTag(TreeItem* parent) : TreeItem(parent)
    {
    }
public:
    virtual void read(QDataStream& in) = 0;
};
class TreeItemNbtTagEnd : public TreeItemNbtTag
{
public:
    TreeItemNbtTagEnd(TreeItem* parent) : TreeItemNbtTag(parent)
    {
    }
public:
    virtual void read(QDataStream& in) override
    {
        Q_UNUSED(in);
    }
};
class TreeItemNbtTagByte : public TreeItemNbtTag
{
    quint8 value;
public:
    TreeItemNbtTagByte(TreeItem* parent);
    ~TreeItemNbtTagByte();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};
class TreeItemNbtTagShort : public TreeItemNbtTag
{
    quint16 value;
public:
    TreeItemNbtTagShort(TreeItem* parent);
    ~TreeItemNbtTagShort();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};
class TreeItemNbtTagInt : public TreeItemNbtTag
{
    quint32 value;
public:
    TreeItemNbtTagInt(TreeItem* parent);
    ~TreeItemNbtTagInt();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};
class TreeItemNbtTagLong : public TreeItemNbtTag
{
    quint64 value;
public:
    TreeItemNbtTagLong(TreeItem* parent);
    ~TreeItemNbtTagLong();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};
class TreeItemNbtTagFloat : public TreeItemNbtTag
{
    float value;
public:
    TreeItemNbtTagFloat(TreeItem* parent);
    ~TreeItemNbtTagFloat();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};
class TreeItemNbtTagDouble : public TreeItemNbtTag
{
    double value;
public:
    TreeItemNbtTagDouble(TreeItem* parent);
    ~TreeItemNbtTagDouble();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};
class TreeItemNbtTagByteArray : public TreeItemNbtTag
{
public:
    QVector<quint8> data;
public:
    TreeItemNbtTagByteArray(TreeItem* parent);
    ~TreeItemNbtTagByteArray();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};
class TreeItemNbtTagString : public TreeItemNbtTag
{
public:
    QString value;
public:
    TreeItemNbtTagString(TreeItem* parent);
    ~TreeItemNbtTagString();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};
class TreeItemNbtTagList : public TreeItemNbtTag
{
public:
    TreeItemNbtTagList(TreeItem* parent);
    ~TreeItemNbtTagList();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};
class TreeItemNbtTagCompound : public TreeItemNbtTag
{
public:
    TreeItemNbtTagCompound(TreeItem* parent);
    ~TreeItemNbtTagCompound();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};
class TreeItemNbtTagIntArray : public TreeItemNbtTag
{
public:
    QVector<quint32> data;
public:
    TreeItemNbtTagIntArray(TreeItem* parent);
    ~TreeItemNbtTagIntArray();
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void read(QDataStream& in) override;
};

class TreeItemJsonFile : public TreeItem
{
public:
    QString name;
public:
    TreeItemJsonFile(TreeItem* parent, const QString& folder, const QString& fileName);
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class TreeItemJsonPair : public TreeItem
{
public:
    QString name;
    QString value;
public:
    TreeItemJsonPair(TreeItem* parent, const QString& nameIn, const QString& valueIn);
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};
class TreeItemJsonArray : public TreeItem
{
public:
    QString name;
public:
    TreeItemJsonArray(TreeItem* parent, const QString& nameIn)
        : TreeItem(parent)
        , name(nameIn)
    {
    }
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};
class TreeItemJsonObject : public TreeItem
{
public:
    QString name;
public:
    TreeItemJsonObject(TreeItem* parent, const QString& nameIn)
        : TreeItem(parent)
        , name(nameIn)
    {
        sort();
    }
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class TreeItemRegionFile : public TreeItem
{
public:
    QString name;
public:
    TreeItemRegionFile(TreeItem* parent, const QString& folder, const QString& fileName);
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};
class QFile;
class TreeItemRegionChunk : public TreeItem
{
public:
    int chunkX;
    int chunkZ;
    bool isCompressed;
public:
    TreeItemRegionChunk(TreeItemRegionFile* parent, QFile& file, int x, int z, quint32 location, quint32 timestamps);
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

#endif // TREEITEM_H
