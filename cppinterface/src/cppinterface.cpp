#include "cppinterface.h"
#include "bswmdfindertask.h"
#include "ecucparsertask.h"
#include "referenceresolvertask.h"
#include "projectloadertask.h"
#include "dpa_file.h"
#include "projectinfo.h"
#include <QThreadPool>
#include <QDateTime>
#include <chrono>

CppInterface::CppInterface(QObject *parent):
    QObject(parent)
{

}

void CppInterface::openEcucFolder(QString path)
{
    emit ecucFolderOpened(path);
}

void CppInterface::openEcucFile(QString path)
{
    emit ecucFileOpened(path);
}

void CppInterface::forwardProjectLoaded(ProjectInfo* project)
{
    emit projectLoaded(project);
}

void CppInterface::openDpaFile(QUrl path)
{
    dpa_file dpa(path.toLocalFile().toStdString());
    ProjectLoaderTask* loader = new ProjectLoaderTask(dpa.name(), dpa.sip_path(), dpa.ecuc_files(), dpa.dpa_path());
    QThreadPool::globalInstance()->start(loader);
    using namespace std::chrono_literals;
    if(loader->result_future().wait_for(120s) == std::future_status::ready)
    {
        ProjectInfo* proj = loader->result_future().get();
        emit projectLoaded(proj);
    }
    else
    {
        // Error - Loader did not finish in 2 minutes
        std::cout << "Error: Could not load project" << std::endl;
    }
}