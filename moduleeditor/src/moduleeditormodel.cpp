#include "moduleeditormodel.h"

#include "arxml_tags.hpp"
#include <set>
#include <iostream>

ModuleEditorModel::ModuleEditorModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

WorkspaceController* ModuleEditorModel::workspace() const
{

}

void ModuleEditorModel::setWorkspace(WorkspaceController* workspace)
{
    workspace_ = workspace;
    beginResetModel();
    _items.clear();
    endResetModel();
}

int ModuleEditorModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return _items.size();
}

QVariant ModuleEditorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const Parameter& item = *(_items.at(index.row()));
    QVariant returnValue;
    switch(role)
    {
        case NameRole:
            returnValue = QVariant(item.name());
            break;
        case TypeRole:
            returnValue = QVariant(item.type());
            break;
        case ValueRole:
            returnValue = QVariant(item.value());
            break;
        case ChoicesRole:
            returnValue = QVariant(item.choices());
            break;
        case UpperMultiplicityRole:
            returnValue = QVariant(item.upper_multiplicity());
            break;
        case LowerMultiplicityRole:
            returnValue = QVariant(item.lower_multiplicity());
            break;
        default:
            // Return null value
            break;
    }

    return returnValue;
}

bool ModuleEditorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ModuleEditorModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> ModuleEditorModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[TypeRole] = "type";
    names[ValueRole] = "value";
    names[ChoicesRole] = "choices";
    names[UpperMultiplicityRole] = "upperMultiplicity";
    names[LowerMultiplicityRole] = "lowerMultiplicity";
    return names;
}

Q_INVOKABLE void ModuleEditorModel::loadContainer(QString path)
{
    if(workspace_ != nullptr)
    {
        // The path might have been extended by a bswmd container (syntax: /a/b/c/d:xyz)
        // If the path contains a ":"
        std::shared_ptr<EcuProjectModel> ecuproject_ = workspace_->project(projectId_);
        auto ecuc_container = ecuproject_->model()->node_by_path(path.toStdString());
        if(ecuc_container != nullptr)
        {
            auto ecuc_param_cont = std::static_pointer_cast<AutosarGen::param_container_val>(ecuc_container);
            if(ecuc_param_cont->definition_ref()->is_resolved())
            {
                beginResetModel();
                _items.clear();
                switch(ecuc_container->type())
                {
                    case arx::kModuleCfg:
                    case arx::kParamContainerVal:
                    case arx::kChoiceContainerVal:
                    {
                        // First go through all parameters in the bswmd container.
                        if(ecuc_param_cont->definition_ref()->is_resolved())
                        {
                            // A basic software module definition was found:
                            loadResolvedContainer_(ecuc_param_cont, ecuc_param_cont->definition_ref()->target());
                        }
                        else
                        {
                            // No basic software module definition found. Only show basic information.
                            loadUnresolvedContainer_(ecuc_param_cont);
                        }
                        
                    }
                    case arx::kNumParamVal:
                    case arx::kTextParamVal:
                    case arx::kReferenceVal:
                    case arx::kChoiceReferenceVal:
                    {
                        //TODO: Not handled at the moment.
                        break;
                    }
                }
                endResetModel();
            }
            else
            {
                std::cout << "No BSWMD found for: " << ecuc_param_cont->path() << ". Not supported yet." << std::endl;
            }

        }
    }
}

void ModuleEditorModel::loadResolvedContainer_(std::shared_ptr<AutosarGen::param_container_val> ecuc_param, std::shared_ptr<arx::referrable> bswmd_param)
{
    for(auto bswmd_child: bswmd_param->children())
    {
        switch(bswmd_child->type())
        {
            case arx::kEnumParamDef:
            {
                AutosarGen::text_param_val* ecuc_def = nullptr;
                for(auto& txt_param: ecuc_param->textual_params())
                {
                    if(txt_param.definition_ref()->target() == bswmd_child)
                    {
                        ecuc_def = &txt_param;
                        break;
                    }
                }
                _items.push_back(std::make_unique<EnumerationParameter>(bswmd_child, ecuc_def));
                break;
            }
            case arx::kStringDef:
            case arx::kFunctionNameDef:
            {
                AutosarGen::text_param_val* ecuc_def = nullptr;
                for(auto& txt_param: ecuc_param->textual_params())
                {
                    if(txt_param.definition_ref()->target() == bswmd_child)
                    {
                        ecuc_def = &txt_param;
                        break;
                    }
                }
                _items.push_back(std::make_unique<StringParameter>(bswmd_child, ecuc_def));
                break;
            }
            case arx::kIntParamDef:
            {
                AutosarGen::num_param_val* ecuc_def = nullptr;
                for(auto& num_param: ecuc_param->numerical_params())
                {
                    if(num_param.definition_ref()->target() == bswmd_child)
                    {
                        ecuc_def = &num_param;
                        break;
                    }
                }
                _items.push_back(std::make_unique<IntParameter>(bswmd_child, ecuc_def));
                break;
            }
            case arx::kFloatParamDef:
            {
                AutosarGen::num_param_val* ecuc_def = nullptr;
                for(auto& num_param: ecuc_param->numerical_params())
                {
                    if(num_param.definition_ref()->target() == bswmd_child)
                    {
                        ecuc_def = &num_param;
                        break;
                    }
                }
                _items.push_back(std::make_unique<FloatParameter>(bswmd_child, ecuc_def));
                break;
            }
            case arx::kBoolParamDef:
            {
                AutosarGen::num_param_val* ecuc_def = nullptr;
                for(auto& num_param: ecuc_param->numerical_params())
                {
                    if(num_param.definition_ref()->target() == bswmd_child)
                    {
                        ecuc_def = &num_param;
                        break;
                    }
                }
                _items.push_back(std::make_unique<BoolParameter>(bswmd_child, ecuc_def));
                break;
            }
            case arx::kSymbolicNameReferenceDef:
            case arx::kReferenceDef:
            case arx::kChoiceReferenceDef:
            {
                std::vector<AutosarGen::reference_val*> ecuc_defs;
                for(auto& ref_param: ecuc_param->references())
                {
                    if(ref_param.definition_ref()->target() == bswmd_child)
                    {
                        ecuc_defs.push_back(&ref_param);
                    }
                }
                _items.push_back(std::make_unique<Reference>(bswmd_child, ecuc_defs));
                break;
            }
            case arx::kEnumLiteralDef:
            default:
                // Do nothing.
                break;
        }
    }
}
void ModuleEditorModel::loadUnresolvedContainer_(std::shared_ptr<AutosarGen::param_container_val> ecuc_param)
{
    // Not implemented yet.
}
