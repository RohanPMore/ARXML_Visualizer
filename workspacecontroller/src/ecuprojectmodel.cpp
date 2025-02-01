#include "ecuprojectmodel.h"

EcuProjectModel::EcuProjectModel(ProjectInfo* projectInfo)
{
    if(projectInfo != nullptr)
    {
        projectInfo_ = *projectInfo;
    }
}

QString EcuProjectModel::name() const
{
    return projectInfo_.name();
}

std::vector<BswModule>& EcuProjectModel::modules()
{ 
    return projectInfo_.modules();
}

std::shared_ptr<arx::armodel> EcuProjectModel::model()
{
    return projectInfo_.model();
}