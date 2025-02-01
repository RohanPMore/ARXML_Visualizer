#pragma once

#include "arxml_parser/armodel.hpp"
#include "arxml_parser/arxml_file.hpp"
#include "bswmdfindertask.h"
#include "ecucparsertask.h"
#include "module_cfg.hpp"
#include <QRunnable>
#include <future>
#include <unordered_map>
#include <filesystem>
#include <memory>
#include <atomic>
#include "projectinfo.h"


class ProjectLoaderTask : public QRunnable
{
public:

    ProjectLoaderTask(std::string_view project_name, const std::filesystem::path& sip_path, const std::vector<std::filesystem::path>& ecuc_files, const std::filesystem::path& project_file);
    std::future<ProjectInfo*>& result_future() { return result_future_; }
    void run() override;
private:
    const std::string project_name_;
    const std::filesystem::path sip_path_;
    const std::vector<std::filesystem::path> ecuc_files_;
    const std::filesystem::path project_file_;
    std::promise<ProjectInfo*> result_promise_;
    std::future<ProjectInfo*> result_future_;
};
