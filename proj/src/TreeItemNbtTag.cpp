#include "TreeItem.h"
#include <QByteArray>
#include <QDataStream>
#include <QMimeData>
#include <QClipboard>
#include <QApplication>

TreeItem* TreeItemNbtTag::markDirty()
{
    TreeItem* tmp = this;

    while(tmp != nullptr)
    {
        if(dynamic_cast<TreeItemNbtFile*>(tmp))
            return tmp;
        if(dynamic_cast<TreeItemRegionFile*>(tmp))
            return tmp;

        tmp = tmp->parent;
    }
    return nullptr;
}

QString TreeItemNbtTag::getName() const
{
    if(dynamic_cast<TreeItemNbtTagList*>(parent))
    {
        return QString();
    }
    return name;
}

bool TreeItemNbtTag::canRename() const
{
    return dynamic_cast<TreeItemNbtTagList*>(parent) == nullptr;
}

bool TreeItemNbtTag::canMoveUp() const
{
    return dynamic_cast<const TreeItemNbtTagList*>(parent) != nullptr
            && parent->children.indexOf(const_cast<TreeItemNbtTag*>(this)) > 0;
}

bool TreeItemNbtTag::canMoveDown() const
{
    return dynamic_cast<const TreeItemNbtTagList*>(parent) != nullptr
            && parent->children.indexOf(const_cast<TreeItemNbtTag*>(this)) < parent->children.size()-1;
}

void TreeItemNbtTag::cutItem()
{
    copyItem();
    delete this;
}

void TreeItemNbtTag::copyItem()
{
    QByteArray data;
    QDataStream dataStream(&data, QIODevice::WriteOnly);

    dataStream << (quint8)nbtType();
    writeStringUTF8(name, dataStream);
    writeNbt(dataStream);
    dataStream << (quint8)NBTTAG_END;

    QMimeData* mime = new QMimeData();
    mime->setData("minecraft/nbt-tag", data);
    qApp->clipboard()->setMimeData(mime);
}

void TreeItemNbtTag::pasteIntoItem()
{
    parseNbtTagFromClipboard(this);
}
