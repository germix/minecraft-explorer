#include "TreeItem.h"

TreeItem::TreeItem(TreeItem* ___parent)
    : parent(___parent)
{
    if(parent)
        parent->children.append(this);
}

TreeItem::~TreeItem()
{
    //
    // Remover del padre
    //
    if(parent)
    {
        parent->children.removeOne(this);
    }
    //
    // Eliminar hijos
    //
    for(int i = 0; i < children.count(); i++)
    {
        TreeItem* child = children.at(i);
        // ...
        child->parent = nullptr;
        // ...
        delete child;
    }
    children.clear();
}

static bool treeItemLessThan(TreeItem* item1, TreeItem* item2)
{
    if(dynamic_cast<TreeItemFolder*>(item1) != nullptr
        && dynamic_cast<TreeItemFolder*>(item2) == nullptr)
    {
        return true;
    }
    if(dynamic_cast<TreeItemFolder*>(item2) != nullptr
        && dynamic_cast<TreeItemFolder*>(item1) == nullptr)
    {
        return false;
    }
    return item1->getLabel() < item2->getLabel();
}

void TreeItem::sort()
{
    std::sort(children.begin(), children.end(), treeItemLessThan);
}

void TreeItem::clear()
{
    for(int i = 0; i < children.size(); i++)
    {
        children[i]->parent = nullptr;
        delete children[i];
    }
    children.clear();
}
