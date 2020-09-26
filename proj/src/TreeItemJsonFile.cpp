#include "TreeItem.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


void parseJsonArray(TreeItem* parent, QJsonArray arr);
void parseJsonObject(TreeItem* parent, QJsonObject obj);

void parseJsonValue(TreeItem* parent, QString key, QJsonValue value)
{
    if(value.isNull())
    {
        new TreeItemJsonPair(parent, key, "null");
    }
    else if(value.isBool())
    {
        new TreeItemJsonPair(parent, key, value.toBool() == true ? "true" : "false");
    }
    else if(value.isDouble())
    {
        new TreeItemJsonPair(parent, key, QString::number(value.toDouble()));
    }
    else if(value.isString())
    {
        new TreeItemJsonPair(parent, key, value.toString());
    }
    else if(value.isArray())
    {
        parseJsonArray(new TreeItemJsonArray(parent, key), value.toArray());
    }
    else if(value.isObject())
    {
        parseJsonObject(new TreeItemJsonObject(parent, key), value.toObject());
    }
}

void parseJsonArray(TreeItem* parent, QJsonArray arr)
{
    for(int i = 0; i < arr.size(); i++)
    {
        parseJsonValue(parent, "", arr.at(i));
    }
}

void parseJsonObject(TreeItem* parent, QJsonObject obj)
{
    for(QJsonObject::ConstIterator it = obj.constBegin(); it != obj.constEnd(); it++)
    {
        parseJsonValue(parent, it.key(), it.value());
    }
}

TreeItemJsonFile::TreeItemJsonFile(TreeItem* parent, const QString& folder, const QString& fileName)
    : TreeItem(parent)
{
    QFile file(folder + "/" + fileName);

    name = fileName;

    if(file.open(QFile::ReadOnly))
    {
        QJsonDocument json = QJsonDocument::fromJson(file.readAll());

        if(json.isObject())
        {
            parseJsonObject(this, json.object());
        }
    }
}

QIcon TreeItemJsonFile::getIcon() const
{
    return QIcon(":/images/treeitem-json-file.png");
}

QString TreeItemJsonFile::getLabel() const
{
    return name;
}
