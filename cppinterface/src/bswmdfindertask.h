#pragma once

#include "rapidxml/rapidxml.hpp"
#include <QRunnable>
#include <future>
#include <unordered_map>
#include <filesystem>
#include <memory>
#include <optional>

class BswmdFinderTask : public QRunnable
{
public:
    typedef std::unordered_map<std::string, std::filesystem::path> bswmd_map_type;

    BswmdFinderTask(const std::filesystem::path& sip_folder);

    std::future<std::shared_ptr<bswmd_map_type> > bswmd_map() { return bswmd_map_promise_.get_future(); }
    void run() override;

private:
    std::filesystem::path sip_folder_;
    std::promise<std::shared_ptr<bswmd_map_type> > bswmd_map_promise_;
    std::shared_ptr<bswmd_map_type> bswmd_map_;

    void process_bswmd_folder_(const std::filesystem::path& folder, std::shared_ptr<bswmd_map_type> result);
    void contained_module_def_(const std::filesystem::path& arxml_file, std::vector<std::string>& module_paths);
    void find_module_def_(rapidxml::xml_node<>* node, std::vector<rapidxml::xml_node<>*>& module_defs);
    std::string get_shortname_path_(rapidxml::xml_node<>* node);
};
