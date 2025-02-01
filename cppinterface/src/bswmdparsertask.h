#pragma once

#include "arxml_parser/armodel.hpp"
#include "arxml_parser/arxml_file.hpp"
#include <QRunnable>
#include <future>
#include <unordered_map>
#include <filesystem>
#include <memory>


class BswmdParserTask : public QRunnable
{
public:
    BswmdParserTask(const std::filesystem::path& bswmd_file, std::shared_ptr<arx::armodel> model);

    std::future<std::shared_ptr<arx::arxml_file>> bswmd_file() { return bswmd_file_promise_.get_future(); }

    void run() override;

private:
    const std::filesystem::path& bswmd_path_;
    std::shared_ptr<arx::armodel> model_;
    std::shared_ptr<arx::arxml_file> bswmd_file_;
    std::promise<std::shared_ptr<arx::arxml_file> > bswmd_file_promise_;
};
