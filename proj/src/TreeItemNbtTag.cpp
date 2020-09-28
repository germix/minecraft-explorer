#include "TreeItem.h"

TreeItem* TreeItemNbtTag::markDirty()
{
    TreeItem* tmp = this;

    while(tmp != nullptr)
    {
        if(dynamic_cast<TreeItemNbtFile*>(tmp))
            return tmp;
        if(dynamic_cast<TreeItemRegionFile*>(tmp))
            return tmp;
        if(dynamic_cast<TreeItemRegionChunk*>(tmp))
        {
            ((TreeItemRegionChunk*)tmp)->modified = true;
        }

        tmp = tmp->parent;
    }
    return nullptr;
}

bool TreeItemNbtTag::canRename() const
{
    return dynamic_cast<TreeItemNbtTagList*>(parent) == nullptr;
}
