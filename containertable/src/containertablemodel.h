#pragma once

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QtQml/qqmlregistration.h>
#include <memory>
#include <vector>
#include <QStringList>

#include "workspacecontroller.h"

class ContainerTableModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(WorkspaceController* workspace READ workspace WRITE setWorkspace REQUIRED)
    Q_PROPERTY(QString ecucPath READ ecucPath WRITE setEcucPath)
    Q_PROPERTY(int projectId READ projectId WRITE setProjectId)
    QML_ELEMENT
    QML_ADDED_IN_VERSION(1, 1)

public:
    ContainerTableModel(QObject* parent = nullptr);
    void setWorkspace(WorkspaceController* workspace);
    WorkspaceController* workspace();

    void setEcucPath(QString ecucPath);
    QString ecucPath();

    void setProjectId(int projectId);
    int projectId();

    Q_INVOKABLE void setFilter(QString filter);
    Q_INVOKABLE QString ecucPathFromRow(const int row);
    Q_INVOKABLE int columnWidth(const int column);

private:
    class ContainerTableBaseModel : public QAbstractTableModel
    {
        public:
        int rowCount(const QModelIndex& = QModelIndex()) const override;

        int columnCount(const QModelIndex& = QModelIndex()) const override;

        QVariant data(const QModelIndex& index, int role) const override;

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

        QHash<int, QByteArray> roleNames() const override;

        QStringList columnHeaders;
        QStringList rowHeaders;
        std::vector<QStringList> rows;
        std::vector<int> columnWidths;
    };

    ContainerTableBaseModel basemodel_;
    WorkspaceController* workspace_;
    QString ecucPath_;
    QString bswmdPath_;
    bool filterEnabled_;   
    int projectId_; 

    void updateModel_();
};
