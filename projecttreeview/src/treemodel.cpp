#include "treemodel.h"
#include "arxml_tags.hpp"
#include "param_container_def.hpp"
#include "param_container_val.hpp"
#include <iostream>
#include <algorithm>

TreeModel::TreeModel(QObject *parent) :
    QAbstractItemModel(parent),
    rootItem_(std::make_unique<TreeItem>()),
    projectId_(-1),
    workspace_(nullptr)
{
}

TreeModel::~TreeModel() {}

void TreeModel::moduleLoaded(QString ecucPath)
{
}

WorkspaceController* TreeModel::workspace()
{
    return workspace_;
}

void TreeModel::setWorkspace(WorkspaceController* workspace)
{
    workspace_ = workspace;
    updateModel();
}

int TreeModel::projectId()
{
    return projectId_;
}

void TreeModel::setProjectId(int id)
{
    projectId_ = id;
    updateModel();
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
    int count = 0;
    if (!parent.isValid())
    {
        count = rootItem_->childCount();
    }
    else
    {
        TreeItem* p = static_cast<TreeItem*>(parent.internalPointer());
        count = p->childCount();
    }

    return count;
}

int TreeModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 1;
}

QModelIndex TreeModel::index(const int row, const int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *item = rootItem_.get();
    if (parent.isValid())
        item = static_cast<TreeItem *>(parent.internalPointer());

    auto child = item->child(row);
    if (child)
        return createIndex(row, column, child);

    return QModelIndex();
}
QModelIndex TreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem* childItem = static_cast<TreeItem *>(index.internalPointer());
    TreeItem* parentItem = childItem->parent();

    if (parentItem == rootItem_.get())
        return QModelIndex();

    return createIndex(parentItem->index(), 0, parentItem);
}

QVariant TreeModel::data(const QModelIndex& index, const int role) const
{
    QVariant returnValue;

    if(index.isValid())
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        switch(role)
        {
            case TitleRole:
                returnValue = item->title();
                break;
            case TypeRole:
                returnValue = item->type();
                break;
            case EcucPathRole:
                returnValue = item->ecucPath();
                break;
            default:
                // Return null value
                break;
        }
    }
    return returnValue;
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    static QHash<int, QByteArray> names = {
        { TitleRole, "title" },
        { TypeRole, "type" },
        { EcucPathRole, "ecucPath"}
    };
    return names;
}

QString TreeModel::ecucPath(const QModelIndex &index) const
{
    if(index.isValid())
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        return item->ecucPath();
    }
    return QString();
}

QModelIndex TreeModel::indexFromEcucPath(QString ecucPath) const
{
    return findItemInSubTree_(ecucPath, rootItem_.get());
}

QModelIndex TreeModel::findItemInSubTree_(QString ecucPath, TreeItem::ItemWeakPtr currentItem) const
{
    for(int i=0;i<currentItem->childCount();i++)
    {
        TreeItem::ItemWeakPtr child = currentItem->child(i);
        if(child->ecucPath().isEmpty())
        {
            // This is a container "folder". Recurse
            auto index = findItemInSubTree_(ecucPath, child);
            if(index.isValid())
            {
                return index;
            }
        }
        else
        {
            if(ecucPath.startsWith(child->ecucPath()))
            {
                if(ecucPath.length() == child->ecucPath().length())
                {
                    // This is the item we are searching for
                    return createIndex(i, 0, child);
                }
                else
                {
                    // Check if the next character in the input ecuc path is a path seperator
                    if(ecucPath.length() > child->ecucPath().length() + 1)
                    {
                        if(ecucPath[child->ecucPath().length()] == '/')
                        {
                            // The EcuC path we are searching for is longer than the current path
                            // and there is a path separator after the current child.
                            // Continue to search for the full path in the children:
                            auto index =  findItemInSubTree_(ecucPath, child);
                            if(index.isValid())
                            {
                                return index;
                            }
                        }
                        else
                        {
                            // The EcuC path is longer than the current child but the next character is NOT a path separator.
                            // Either there is a reference to a BSWMD container (format: "/a/b/c/d : BSWMD_Container")
                            // at the end of the path or this is a deadend.
                            auto index_sep = ecucPath.lastIndexOf(QChar(':'));
                            if(index_sep > -1)
                            {
                                // The ecucPath contains a BSWMD container. Extract the name of the container
                                auto last_space = ecucPath.lastIndexOf(QChar(' '));
                                qsizetype start_of_name = index_sep + 1;
                                if(last_space > -1)
                                {
                                    start_of_name = last_space + 1;
                                }
                                auto bswmd_name = ecucPath.sliced(start_of_name);
                                
                                // Search in children for the bswmd_name:
                                for(int j=0;j<child->childCount();j++)
                                {
                                    TreeItem::ItemWeakPtr grandchild = child->child(j);
                                    if(grandchild->title().toString() == bswmd_name)
                                    {
                                        // This is the item we are searching for
                                        return createIndex(j, 0, grandchild);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return QModelIndex();
}

TreeItem::ItemWeakPtr TreeModel::addTreeItem(TreeItem::ItemWeakPtr parent, const QString& data, const TreeItemType::Type type)
{
    beginInsertRows(QModelIndex(),
                   parent->childCount() - 1,
                   parent->childCount() - 1);
    auto newItem = parent->appendChild(data, type);
    endInsertRows();

    return newItem;
}

void TreeModel::removeItemByIndex(TreeItem::ItemWeakPtr parent, int index)
{
    if (parent->child(index))
    {
        beginRemoveRows(QModelIndex(), parent->childCount() - 1, parent->childCount());
        parent->removeChild(index);
        endRemoveRows();
    }
}

TreeItem::ItemWeakPtr TreeModel::rootItem() const
{
    return rootItem_.get();
}

QModelIndex TreeModel::rootIndex()
{
    return QModelIndex();
}

void TreeModel::updateModel()
{
    if(projectId_ >= 0 && workspace_ != nullptr)
    {
        std::shared_ptr<EcuProjectModel> project = workspace_->project(projectId_);
        if(project != nullptr)
        {
            beginResetModel();
            // Sort modules by shortname
            struct
            {
                bool operator()(const BswModule& a, const BswModule& b) const { return a.name() < b.name(); }
            }name_sort;
            std::vector<BswModule> modules(project->modules().begin(), project->modules().end());
            std::sort(modules.begin(), modules.end(), name_sort);
            for(auto& module: modules)
            {
                if(module.config()->definition_ref()->is_resolved())
                    addSubTree_(rootItem_.get(), module.config(), module.config()->definition_ref()->target());
            }
            endResetModel();
        }
    }
}

void TreeModel::addSubTree_(TreeItem::ItemWeakPtr modelParent, std::shared_ptr<arx::referrable> ecuc_node, std::shared_ptr<arx::referrable> bswmd_node)
{
    TreeItem::ItemWeakPtr newParent = modelParent;
    switch(bswmd_node->type())
    {
        case arx::kModuleDef:
        {
            newParent = modelParent->appendChild(QString::fromStdString(std::string(ecuc_node->shortname())), TreeItemType::MODULE, QString::fromStdString(std::string(ecuc_node->path())));
            // Sort children by shortname
            struct
            {
                bool operator()(std::shared_ptr<arx::referrable> a, std::shared_ptr<arx::referrable> b) const { return a->shortname() < b->shortname(); }
            }shortname_sort;
            std::vector<std::shared_ptr<arx::referrable> > children(bswmd_node->children().begin(), bswmd_node->children().end());
            std::sort(children.begin(), children.end(), shortname_sort);

            // Iterate over all children of the EcuC module config
            for(auto child: children)
            {
                // We only care about parameter containers for the tree view:
                if(child->type() == arx::kParamContainerDef)
                {
                    auto bswmd_container = std::static_pointer_cast<AutosarGen::param_container_def>(child);
                    addSubTree_(newParent, ecuc_node, bswmd_container);

                }
                else if(child->type() == arx::kChoiceContainerDef)
                {
                    auto bswmd_container = std::static_pointer_cast<AutosarGen::param_container_def>(child);
                    addSubTree_(newParent, ecuc_node, bswmd_container);
                }
                else
                {
                    // Do nothing.
                }
                
            }
            break;
        }
        case arx::kChoiceContainerDef:
        case arx::kParamContainerDef:
        {
            // Containers are a two step process. First the group is created for all container instances, then the container instances are added.
            // Check if this is already the second step (create ECUC container instances)
            if(ecuc_node->type() == arx::kParamContainerVal)
            {
                auto container = std::static_pointer_cast<AutosarGen::param_container_val>(ecuc_node);
                if(container->definition_ref()->target() == bswmd_node)
                {
                    
                    newParent = modelParent->appendChild(QString::fromStdString(std::string(ecuc_node->shortname())), TreeItemType::CONTAINER, QString::fromStdString(std::string(ecuc_node->path())));
                    
                     // Sort children by shortname
                    struct
                    {
                        bool operator()(std::shared_ptr<arx::referrable> a, std::shared_ptr<arx::referrable> b) const { return a->shortname() < b->shortname(); }
                    }shortname_sort;
                    std::vector<std::shared_ptr<arx::referrable> > children(bswmd_node->children().begin(), bswmd_node->children().end());
                    std::sort(children.begin(), children.end(), shortname_sort);

                    // Iterate over all children of the bswmd node and recurse.
                    for(auto child: children)
                    {
                        // We only care about parameter containers for the tree view:
                        if(child->type() == arx::kParamContainerDef)
                        {
                            auto bswmd_container = std::static_pointer_cast<AutosarGen::param_container_def>(child);
                            addSubTree_(newParent, ecuc_node, bswmd_container);

                        }
                        else if(child->type() == arx::kChoiceContainerDef)
                        {
                            auto bswmd_container = std::static_pointer_cast<AutosarGen::param_container_def>(child);
                            addSubTree_(newParent, ecuc_node, bswmd_container);
                        }
                        else
                        {
                            // Do nothing.
                        }
                    }
                    return;
                }
            }

            // This is the first step in parsing the container def bswmd node.
            // If the upper multiplicity is > 1 create a tree node with the name of the bswmd node as a group for the ecuc containers:
            auto container_def = std::static_pointer_cast<AutosarGen::param_container_def>(bswmd_node);
            if(container_def->upper_multiplicity() > 1)
            {
                
                newParent = modelParent->appendChild(QString::fromStdString(std::string(bswmd_node->shortname())), TreeItemType::CONTAINERTYPE, QString::fromStdString(std::string(ecuc_node->path())));
            }

            // Sort children by shortname
            struct
            {
                bool operator()(std::shared_ptr<arx::referrable> a, std::shared_ptr<arx::referrable> b) const { return a->shortname() < b->shortname(); }
            }shortname_sort;
            std::vector<std::shared_ptr<arx::referrable> > children(ecuc_node->children().begin(), ecuc_node->children().end());
            std::sort(children.begin(), children.end(), shortname_sort);

            // Iterate over all instances of this container and recurse for them
            for(auto child: children)
            {
                // Skip children which are not containers
                if(child->type() == arx::kParamContainerVal)
                {
                    auto container = std::static_pointer_cast<AutosarGen::param_container_val>(child);
                    if(container->definition_ref()->target() == bswmd_node)
                    {
                        // Recurse
                        addSubTree_(newParent, container, bswmd_node);
                    }
                }
            }
            break;
        }
        case arx::kEnumParamDef:
        case arx::kEnumLiteralDef:
        case arx::kIntParamDef:
        case arx::kBoolParamDef:
        case arx::kReferenceDef:
        default:
            // We reached the parameters.
            // No further recursion is required.
            break;
    }  
    
}