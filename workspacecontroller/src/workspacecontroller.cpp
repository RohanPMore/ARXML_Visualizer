#include "workspacecontroller.h"
#include "dpa_file.h"
#include "rapidxml/rapidxml.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <array>

WorkspaceController::WorkspaceController(QObject *parent):
    QObject(parent)
{
}

void WorkspaceController::projectParsed(ProjectInfo* projectInfo)
{
    //_active_projects.emplace(std::make_pair(projectInfo->id(), std::make_shared<EcuProjectModel>(projectInfo)));
    _active_projects.emplace_back(std::make_shared<EcuProjectModel>(projectInfo));
    emit projectLoaded(_active_projects.size()-1);
}