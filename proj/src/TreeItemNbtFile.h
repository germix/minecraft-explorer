#ifndef TREEITEMNBTFILE_H
#define TREEITEMNBTFILE_H
#include "TreeItem.h"

class TreeItemNbtFile : public TreeItem
{
public:
    bool canFetchData;
    QString fileName;
    QString parentFolderPath;
    int compressionMethod;
public:
    TreeItemNbtFile(TreeItem* parent, const QString& fileNameIn, const QString& parentFolderPathIn);
public:
    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
    virtual void fetchMore() override;
    virtual bool canFetchMore() const override;
};

#endif // TREEITEMNBTFILE_H
