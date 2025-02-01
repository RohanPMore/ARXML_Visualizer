#pragma once

#include <QVariant>
#include <QQmlEngine>

#include <memory>
#include <vector>

class TreeItemType : public QObject
{
    Q_OBJECT
    QML_UNCREATABLE("TreeItemType is not instantiable. Use the contained enum.")
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit TreeItemType(QObject *parent = nullptr);
    enum Type
    {
        MODULE = 0,
        CONTAINERTYPE,
        CONTAINER
    };
    Q_ENUM(Type)
};

class TreeItem
{
public:
    using ItemWeakPtr = TreeItem*;
    using ItemPtr = std::unique_ptr<TreeItem>;
    explicit TreeItem(ItemWeakPtr parent = nullptr, const QString& data = QString(), const TreeItemType::Type type = TreeItemType::MODULE, QStringView ecucPath = QStringView());
    ~TreeItem();

    int index() const;
    int childCount() const;
    QString ecucPath() const;

    ItemWeakPtr parent();
    ItemWeakPtr child(int index);

    int childIndexByItem(const TreeItem* const item) const;

    ItemWeakPtr appendChild(const QString& data, const TreeItemType::Type type, QStringView ecucPath = QStringView());
    void removeChild(int childIndex);

    QVariant title() const;
    QVariant type() const;

private:
    static constexpr size_t _defaultChildrenReserve = 4;

    ItemWeakPtr _parent;
    QVariant _data;
    QVariant _type;
    QString _ecucPath;
    std::vector<ItemPtr> _children;
};