#pragma once

#include "arxml_parser/armodel.hpp"
#include "arxml_parser/arxml_file.hpp"
#include "bswmdfindertask.h"
#include "module_cfg.hpp"
#include <QRunnable>
#include <future>
#include <unordered_map>
#include <filesystem>
#include <memory>
#include <atomic>


class EcucParserTask : public QRunnable
{
public:
    struct EcucParserResult
    {
        std::shared_ptr<arx::arxml_file> ecuc_file;
        std::shared_ptr<arx::arxml_file> bswmd_file;
        std::shared_ptr<AutosarGen::module_cfg> module_cfg;
        std::string_view bswmd_path;
        std::shared_ptr<arx::armodel> model;
    };

    EcucParserTask(const std::filesystem::path& ecuc_path, std::shared_future<std::shared_ptr<BswmdFinderTask::bswmd_map_type> > bswmd_map);
    EcucParserResult result() { return result_future_.get(); }
    std::atomic_bool result_ready()
    { 
        using namespace std::chrono_literals;
        return result_future_.wait_for(0s) == std::future_status::ready;
    }
    void run() override;

private:
    std::filesystem::path ecuc_path_;
    std::shared_future<std::shared_ptr<BswmdFinderTask::bswmd_map_type> > bswmd_map_;
    std::promise<EcucParserResult> result_promise_;
    std::future<EcucParserResult> result_future_;
};
