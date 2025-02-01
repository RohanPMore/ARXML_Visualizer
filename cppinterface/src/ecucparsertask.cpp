#include "ecucparsertask.h"
#include "module_cfg.hpp"
#include "bswmd_parser.hpp"
#include "ecuc_parser.hpp"


EcucParserTask::EcucParserTask(const std::filesystem::path& ecuc_path, std::shared_future<std::shared_ptr<BswmdFinderTask::bswmd_map_type> > bswmd_map):
    ecuc_path_(ecuc_path),
    bswmd_map_(bswmd_map)
{
    result_future_ = result_promise_.get_future();
    setAutoDelete(false);
}

void EcucParserTask::run()
{
    using namespace std::chrono_literals;
    EcucParserResult result;
    result.model = std::make_shared<arx::armodel>();
    result.module_cfg = nullptr;

    // Parse Ecuc file first
    AutosarGen::ecuc_parser ecuc_parser(result.model);
    result.ecuc_file = std::make_shared<arx::arxml_file>();
    result.ecuc_file->open(ecuc_path_);
    ecuc_parser.parse(result.ecuc_file->parse_xml<0>());
    if(ecuc_parser.module_configs().size() > 0)
    {
        result.module_cfg = std::static_pointer_cast<AutosarGen::module_cfg>(ecuc_parser.module_configs().front());
        // Wait for the bswmd_map to be available
        std::future_status status = bswmd_map_.wait_for(60s);
        //std::future_status status = std::future_status::timeout;
        if(status == std::future_status::ready)
        {
            std::shared_ptr<BswmdFinderTask::bswmd_map_type> bswmds = bswmd_map_.get();
            auto bswmds_it = bswmds->find(std::string(result.module_cfg->definition_ref()->targetpath()));
            if(bswmds_it != bswmds->end())
            {
                AutosarGen::bswmd_parser bswmd_parser(result.model);
                result.bswmd_file = std::make_shared<arx::arxml_file>();
                result.bswmd_file->open(bswmds_it->second);
                bswmd_parser.parse(result.bswmd_file->parse_xml<0>());
            }
            else
            {
                std::cout << "Could not find BSWMD for " << result.module_cfg->definition_ref()->targetpath() << std::endl;
            }
        }
        else
        {
            // Error.
            std::cout << "Error: Bswmd map not available." << std::endl;
        }
    }
    result_promise_.set_value(result);
}