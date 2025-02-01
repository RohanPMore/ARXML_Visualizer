#include "ecuprojectmodel.h"

#include <QtQml/qqml.h>
#include <QObject>
#include <optional>
#include <filesystem>
#include <unordered_map>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/any_range.hpp>
#include "projectinfo.h"
#include <cstdint>

class WorkspaceController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit WorkspaceController(QObject *parent = nullptr);
    std::shared_ptr<EcuProjectModel> project(int id)
    {
        if(id >= 0 && id <= _active_projects.size())
        {
            return _active_projects[id];
        }
        else
        {
            return nullptr;
        }
    }

    Q_INVOKABLE QString projectName(int id)
    {
        if(id >= 0 && id <= _active_projects.size())
        {
            return _active_projects[id]->name();
        }
        else
        {
            return QStringLiteral("");
        }
    }

public slots:
    void projectParsed(ProjectInfo* projectInfo); 

signals:
    void addModule(QString ecuc_path, QString bswmd_path); 
    void projectLoaded(int projectId);

private:
    std::vector<std::shared_ptr<EcuProjectModel> > _active_projects;
};