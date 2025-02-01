#include "projectloadertask.h"
#include "module_cfg.hpp"
#include "bswmd_parser.hpp"
#include "ecuc_parser.hpp"
#include <QDateTime>
#include <QThreadPool>

ProjectLoaderTask::ProjectLoaderTask(std::string_view project_name, const std::filesystem::path& sip_path, const std::vector<std::filesystem::path>& ecuc_files, const std::filesystem::path& project_file):
    project_name_(project_name),
    sip_path_(sip_path),
    ecuc_files_(ecuc_files),
    project_file_(project_file)
{
    result_future_ = result_promise_.get_future();
    setAutoDelete(false);
}

void ProjectLoaderTask::run()
{
    qint64 start;
    qint64 end;
    BswmdFinderTask* finder = new BswmdFinderTask(sip_path_);
    auto bswmd_map_future = finder->bswmd_map().share();
    start = QDateTime::currentMSecsSinceEpoch();
    std::cout << start <<  ": Starting parsing config..." << std::endl;
    QThreadPool::globalInstance()->start(finder);

    // Parse EcuC files
    std::list<EcucParserTask*> pending_modules;
    for(const auto& ecuc : ecuc_files_)
    {
        EcucParserTask* parser = new EcucParserTask(ecuc, bswmd_map_future);
        pending_modules.push_back(parser);
        QThreadPool::globalInstance()->start(parser);
    }

    std::shared_ptr<arx::armodel> model = std::make_shared<arx::armodel>();
    ProjectInfo* project = new ProjectInfo(QString::fromStdString(project_name_), project_file_, model);

    // Wait for the parsing to complete.
    while(pending_modules.size() > 0)
    {
        std::list<EcucParserTask*>::iterator i = pending_modules.begin();
        while (i != pending_modules.end())
        {
            if((*i)->result_ready())
            {
                auto results = (*i)->result();
                if(results.module_cfg != nullptr)
                {
                    model->integrate(results.model);
                    project->modules().emplace_back(results.module_cfg, results.ecuc_file, results.bswmd_file);
                }
                delete(*i);
                pending_modules.erase(i++);
            }
        }
        // Wait 200ms before polling again.
        QThread::msleep(200);
    }
    end = QDateTime::currentMSecsSinceEpoch();
    std::cout << end << ": Finished parsing config: " << end-start << "ms" << std::endl;
    start = end;
    std::cout << start << ": Starting resolving references..." << std::endl;

    // Singlethreaded reference resolution (faster than using referenceresolvertask)
    model->resolve_all_references();

    end = QDateTime::currentMSecsSinceEpoch();
    std::cout << end << ": Finished resolving references: " << end-start << "ms" << std::endl;

    result_promise_.set_value(project);
}