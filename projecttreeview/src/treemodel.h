#pragma once

#include <memory>
#include <QAbstractItemModel>

#include "treeitem.h"
#include <QtQml/qqml.h>
#include "workspacecontroller.h"

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(WorkspaceController* workspace READ workspace WRITE setWorkspace)
    Q_PROPERTY(int projectId READ projectId WRITE setProjectId)
    QML_ELEMENT
public slots:
     void moduleLoaded(QString ecucPath);
public:
    enum
    {
        TitleRole = Qt::UserRole,
        TypeRole,
        EcucPathRole
    };

    explicit TreeModel(QObject* parent = nullptr);
    ~TreeModel();

    WorkspaceController* workspace();
    void setWorkspace(WorkspaceController* workspace);

    int projectId();
    void setProjectId(int id);

    int rowCount(const QModelIndex &index) const override;
    int columnCount(const QModelIndex &) const override;
    QModelIndex index(const int row, const int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &childIndex) const override;
    QVariant data(const QModelIndex &index, const int role = 0) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE QString ecucPath(const QModelIndex &index) const;

    Q_INVOKABLE QModelIndex indexFromEcucPath(QString) const;

    Q_INVOKABLE TreeItem::ItemWeakPtr addTreeItem(TreeItem::ItemWeakPtr parent, const QString& data, const TreeItemType::Type type);
    Q_INVOKABLE void removeItemByIndex(TreeItem::ItemWeakPtr parent, int index);

    Q_INVOKABLE TreeItem::ItemWeakPtr rootItem() const;

    Q_INVOKABLE QModelIndex rootIndex();

private:
    TreeItem::ItemPtr rootItem_;
    WorkspaceController* workspace_;
    int projectId_;
    void updateModel();
    void addSubTree_(TreeItem::ItemWeakPtr modelParent, std::shared_ptr<arx::referrable> ecuc_node, std::shared_ptr<arx::referrable> bswmd_node);
    QModelIndex findItemInSubTree_(QString ecucPath, TreeItem::ItemWeakPtr currentItem) const;
};