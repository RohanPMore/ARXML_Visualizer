#include "containertablemodel.h"
#include "arxml_tags.hpp"
#include "param_container_val.hpp"
#include "param_container_def.hpp"
#include "enum_def.hpp"
#include "int_def.hpp"
#include "float_def.hpp"
#include "bool_def.hpp"
#include <unordered_map>

int ContainerTableModel::ContainerTableBaseModel::rowCount(const QModelIndex&) const
{
    return rowHeaders.size();
}

int ContainerTableModel::ContainerTableBaseModel::columnCount(const QModelIndex&) const
{
    return columnHeaders.size()+1;
}

QVariant ContainerTableModel::ContainerTableBaseModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
        case Qt::DisplayRole:
            // return QString("%1, %2").arg(index.column()).arg(index.row());
            if(index.column() == 0)
            {
                return rowHeaders[index.row()];
            }
            else 
            {
                return rows[index.row()][index.column()-1];
            }
        default:
            break;
    }

    return QVariant();
}

QVariant ContainerTableModel::ContainerTableBaseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        switch (role) {
            case Qt::DisplayRole:
            if(section == 0)
            {
                return QString("Shortname");
            }
            else
            {
                return columnHeaders[section-1];
            }
            default:
                break;
        }
    }
    else
    {
        switch (role) {
            case Qt::DisplayRole:
                return rowHeaders[section];
            default:
                break;
        }
    }
}

QHash<int, QByteArray>  ContainerTableModel::ContainerTableBaseModel::roleNames() const
{
    return { {Qt::DisplayRole, "display"} };
}



ContainerTableModel::ContainerTableModel(QObject* parent):
    QSortFilterProxyModel(parent)
{
    this->setSourceModel(&basemodel_);
    this->setFilterKeyColumn(-1);
    projectId_ = -1;
}

void ContainerTableModel::setWorkspace(WorkspaceController* workspace)
{
    if(workspace != nullptr)
    {
        workspace_ = workspace;
        updateModel_();
    }
}

WorkspaceController* ContainerTableModel::workspace()
{
    return workspace_;
}

void ContainerTableModel::setEcucPath(QString ecucPath)
{
    ecucPath_ = ecucPath;
    if(ecucPath_.length() > 0)
    {
        auto index_sep = ecucPath_.lastIndexOf(QChar(':'));
        if(index_sep > -1)
        {
            // The ecucPath contains a BSWMD container. Extract the name of the container
            qsizetype start_of_name = index_sep + 1;
            bswmdPath_ = ecucPath_.sliced(start_of_name);
            ecucPath_ = ecucPath_.sliced(0, start_of_name-1);
            updateModel_();
        }
    }
}

QString ContainerTableModel::ecucPath()
{
    return ecucPath_;
}

void ContainerTableModel::setProjectId(int projectId)
{
    projectId_ = projectId;
    updateModel_();
}

int ContainerTableModel::projectId()
{
    return projectId_;
}

void ContainerTableModel::updateModel_()
{
    if(workspace_ == nullptr ||
      ecucPath_.length() == 0 ||
      bswmdPath_.length() == 0 ||
      projectId_ < 0)
    {
        // Not all necessary properties are set.
        return; 
    }
    
    std::shared_ptr<EcuProjectModel> project = workspace_->project(projectId_);
    auto ecuc_node = project->model()->node_by_path(ecucPath_.toStdString());
    if(ecuc_node == nullptr)
    {
        std::cout << "Error: Did not find ecuc_node " << ecucPath_.toStdString() << std::endl;
        return; 
    }
    std::shared_ptr<arx::referrable> parent_bswmd = nullptr;
    if(ecuc_node->type() == arx::kParamContainerVal)
    {
        auto container = std::static_pointer_cast<AutosarGen::param_container_val>(ecuc_node);
        parent_bswmd = container->definition_ref()->target();
    }
    else if(ecuc_node->type() == arx::kModuleCfg)
    {
        auto container = std::static_pointer_cast<AutosarGen::module_cfg>(ecuc_node);
        parent_bswmd = container->definition_ref()->target();
    }
    else
    {
        // Unsupported type
        std::cout << "Error: Unsupported node type: " << ecuc_node->type() << std::endl;
        return;
    }
    // Find the targeted BSWMD definition:
    auto target_bswmd = parent_bswmd->child(bswmdPath_.toStdString());
    if(target_bswmd == nullptr)
    {
        std::cout << "Error: Could not find bswmd def: " << bswmdPath_.toStdString() << std::endl;
        return;
    }
    beginResetModel();
    basemodel_.columnHeaders.clear();
    QStringList defaultRow;
    std::unordered_map<std::string, int> columnIndices;
    int index = 0;
    basemodel_.columnWidths.clear();
    basemodel_.columnWidths.push_back(300);
    for(auto child: target_bswmd->children())
    {
        if(child->type() != arx::kParamContainerDef && child->type() != arx::kChoiceContainerDef)
        {
            QString parameterName = QString::fromStdString(std::string(child->shortname()));
            basemodel_.columnHeaders.append(parameterName);
            columnIndices[child->path()] = index;
            // Set the value to the default value for a start. This might later be overwritten when looping over the containers:
            QString defaultValue;
            switch(child->type())
            {
                case arx::kFunctionNameDef:
                case arx::kStringDef:
                case arx::kEnumParamDef:
                {
                    auto def = std::static_pointer_cast<AutosarGen::enum_def>(child);
                    defaultValue = QString::fromStdString(std::string(def->default_value()));
                    basemodel_.columnWidths.push_back(200);
                    break;
                }
                case arx::kIntParamDef:
                {
                    auto def = std::static_pointer_cast<AutosarGen::int_def>(child);
                    defaultValue = QString::fromStdString(std::to_string(def->default_value()));
                    basemodel_.columnWidths.push_back(200);
                    break;
                }
                case arx::kFloatParamDef:
                {
                    auto def = std::static_pointer_cast<AutosarGen::float_def>(child);
                    defaultValue = QString::fromStdString(std::to_string(def->default_value()));
                    basemodel_.columnWidths.push_back(200);
                    break;
                }
                case arx::kBoolParamDef:
                {
                    auto def = std::static_pointer_cast<AutosarGen::bool_def>(child);
                    defaultValue = QString::fromStdString(def->default_value() ? std::string("true") : std::string("false"));
                    basemodel_.columnWidths.push_back(200);
                    break;
                }
                case arx::kSymbolicNameReferenceDef:
                case arx::kChoiceReferenceDef:
                case arx::kReferenceDef:
                    // Reference values do not have default values.
                    defaultValue = QString("-");
                    basemodel_.columnWidths.push_back(400);
                    break;
                default:
                    // Unhandled parameter type. Add default width
                    basemodel_.columnWidths.push_back(200);
                    break;
            }
            defaultRow.append(defaultValue);
            index++;
        }
    }

    basemodel_.rowHeaders.clear();
    basemodel_.rows.clear();
    struct
    {
        bool operator()(std::shared_ptr<arx::referrable> a, std::shared_ptr<arx::referrable> b) const { return a->shortname() < b->shortname(); }
    }shortname_sort;
    std::vector<std::shared_ptr<arx::referrable> > children(ecuc_node->children().begin(), ecuc_node->children().end());
    std::sort(children.begin(), children.end(), shortname_sort);
    for(auto child: children)
    {
        // Only consider containers
        if(child->type() != arx::kParamContainerVal) break;
        auto child_container = std::static_pointer_cast<AutosarGen::param_container_val>(child);

        // Check if this child is of the requested bswmd type:
        if(child_container->definition_ref()->target() == target_bswmd)
        {
            basemodel_.rowHeaders.append(QString::fromStdString(std::string(child->shortname())));
            // Initialize row with default values
            QStringList newRow(defaultRow);

            // Go through all of the parameters:
            for(auto& param: child_container->numerical_params())
            {
                auto index_iterator = columnIndices.find(std::string(param.definition_ref()->targetpath()));
                if(index_iterator != columnIndices.end())
                {
                    if(param.definition_ref()->target()->type() == arx::kBoolParamDef)
                    {
                        newRow[index_iterator->second] = QString::fromStdString(param.value() ? std::string("true") : std::string("false"));
                    }
                    else if(param.definition_ref()->target()->type() == arx::kFloatParamDef)
                    {
                        newRow[index_iterator->second] = QString::fromStdString(std::to_string(param.value_floating()));
                    }
                    else
                    {
                        newRow[index_iterator->second] = QString::fromStdString(std::to_string(param.value()));
                    }
                    
                }
            }
            for(auto& param: child_container->textual_params())
            {
                auto index_iterator = columnIndices.find(std::string(param.definition_ref()->targetpath()));
                if(index_iterator != columnIndices.end())
                {
                    newRow[index_iterator->second] = QString::fromStdString(std::string(param.value()));
                }
            }
            for(auto& param: child_container->references())
            {
                auto index_iterator = columnIndices.find(std::string(param.definition_ref()->targetpath()));
                if(index_iterator != columnIndices.end())
                {
                    newRow[index_iterator->second] = QString::fromStdString(std::string(param.definition_ref()->targetpath()));
                }
            }
            basemodel_.rows.push_back(newRow);
        }
    }
    endResetModel();
}


Q_INVOKABLE void ContainerTableModel::setFilter(QString filter)
{
    this->setFilterWildcard(filter);
}


Q_INVOKABLE QString ContainerTableModel::ecucPathFromRow(const int row)
{
    return ecucPath_ + "/" + this->headerData(row, Qt::Vertical).toString();
}

Q_INVOKABLE int ContainerTableModel::columnWidth(const int column)
{
    if(column < basemodel_.columnWidths.size())
    {
        return basemodel_.columnWidths[column];
    }
    else
    {
        return 200;
    }
}