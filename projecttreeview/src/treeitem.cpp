#include "treeitem.h"

#include <QDebug>

TreeItemType::TreeItemType(QObject *parent):
    QObject(parent)
{
}

TreeItem::TreeItem(ItemWeakPtr parent, const QString& data, const TreeItemType::Type type, QStringView ecucPath)
    : _parent(parent),
      _data(data),
      _type(type),
      _ecucPath(ecucPath.toString())
{
    _children.reserve(_defaultChildrenReserve);
}

TreeItem::~TreeItem() {}

TreeItem::ItemWeakPtr TreeItem::parent()
{
    return _parent;
}

int TreeItem::childCount() const
{
    return _children.size();
}

QString TreeItem::ecucPath() const
{
    return _ecucPath;
}

TreeItem::ItemWeakPtr TreeItem::child(int index)
{
    return _children.at(index).get();
}

int TreeItem::childIndexByItem(const TreeItem* const item) const
{
    bool found = false;
    size_t index = 0;
    for(;index < _children.size(); index++)
    {
        if(_children[index].get() == item)
        {
            found = true;
            break;
        }
    }

    if(found)
    {
        return index;
    }
    else
    {
        return -1;
    }
}

int TreeItem::index() const
{
    if(_parent == nullptr)
    {
        return 0;
    }
    return _parent->childIndexByItem(this);
}

TreeItem::ItemWeakPtr TreeItem::appendChild(const QString& data, const TreeItemType::Type type, QStringView ecucPath)
{
    _children.push_back(std::make_unique<TreeItem>(this, data, type, ecucPath));
    return _children.back().get();
}

void TreeItem::removeChild(int childIndex)
{
    auto child = _children.begin() + childIndex;
    _children.erase(child);
}

QVariant TreeItem::title() const
{
    return _data;
}

QVariant TreeItem::type() const
{
    return _type;
}