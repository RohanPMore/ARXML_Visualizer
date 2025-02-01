#include "moduleeditor.h"

ModuleEditor::ModuleEditor(QObject *parent) : QObject(parent)
{
}

std::vector<std::unique_ptr<Parameter> >& ModuleEditor::items()
{
    return _items;
}

bool ModuleEditor::setItemAt(int index, const Parameter& item)
{
    if (index < 0 || index > _items.size())
        return false;

    auto oldItem = std::move(_items.at(index));
    bool returnValue = true;
    if(oldItem->name() == item.name() && oldItem->type() == item.type())
        returnValue = false;

    //_items[index] = item; TODO Copy data from item to _items[index];

    // Return borrowed value:
    _items.at(index) = std::move(oldItem);

    return returnValue;
}
