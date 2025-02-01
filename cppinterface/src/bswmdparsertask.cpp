#include "bswmdparsertask.h"
#include "module_cfg.hpp"
#include "bswmd_parser.hpp"

BswmdParserTask::BswmdParserTask(const std::filesystem::path& bswmd_path, arx::model& model):
    bswmd_path_(bswmd_path),
    model_(model)
{
}

void BswmdParserTask::run()
{
    AutosarGen::bswmd_parser bswmd_parser(model_);
    bswmd_file_ = std::make_shared<arx::arxml_file>();
    bswmd_file_->open(bswmd_path_);
    bswmd_parser.parse(bswmd_file_.parse_xml<0>());
    // Signal completion
    bswmd_file_promise_.set_value(bswmd_file_);
}