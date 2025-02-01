#pragma once

#include "parameter.h"

#include <QAbstractListModel>
#include <QtQml/qqmlregistration.h>
#include <memory>
#include <vector>
#include "workspacecontroller.h"
#include "param_container_val.hpp"
#include "param_container_def.hpp"


class ModuleEditor;

class ModuleEditorModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(WorkspaceController* workspace READ workspace WRITE setWorkspace)
    Q_PROPERTY(int projectId MEMBER projectId_)
    QML_ELEMENT

public:
    explicit ModuleEditorModel(QObject* parent = nullptr);

    enum
    {
        NameRole = Qt::UserRole,
        TypeRole,
        ValueRole,
        ChoicesRole,
        UpperMultiplicityRole,
        LowerMultiplicityRole
    };

    // Basic functionality:
    WorkspaceController* workspace() const;
    void setWorkspace(WorkspaceController* workspace);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex& index, const QVariant& value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void loadContainer(QString path);

private:
    std::vector<std::unique_ptr<Parameter> > _items;
    WorkspaceController* workspace_;
    int projectId_;

    void loadResolvedContainer_(std::shared_ptr<AutosarGen::param_container_val> ecuc_param, std::shared_ptr<arx::referrable> bswmd_param);
    void loadUnresolvedContainer_(std::shared_ptr<AutosarGen::param_container_val> ecuc_param);
};
