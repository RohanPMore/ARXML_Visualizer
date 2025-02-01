#include "bswmodule.h"
#include "bswmd_parser.hpp"
#include "ecuc_parser.hpp"

#include <iostream>

BswModule::BswModule(std::shared_ptr<AutosarGen::module_cfg> module_cfg, std::shared_ptr<arx::arxml_file> ecuc_file, std::shared_ptr<arx::arxml_file> bswmd_file):
    module_cfg_(module_cfg),
    ecuc_file_(ecuc_file),
    bswmd_file_(bswmd_file)
{
}

QString BswModule::name() const
{
    return QString::fromStdString(std::string(module_cfg_->shortname()));
}

std::shared_ptr<AutosarGen::module_cfg> BswModule::config()
{
    return module_cfg_;
}

QString BswModule::ecuc_path() const
{
    return QString::fromStdString(module_cfg_->path());
}
