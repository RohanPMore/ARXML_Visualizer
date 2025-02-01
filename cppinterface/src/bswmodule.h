#pragma once
#include <QString>
#include "arxml_parser/armodel.hpp"
#include "arxml_parser/arxml_file.hpp"
#include "module_cfg.hpp"

class BswModule
{
public:
    BswModule(std::shared_ptr<AutosarGen::module_cfg> module_cfg, std::shared_ptr<arx::arxml_file> ecuc_file, std::shared_ptr<arx::arxml_file> bswmd_file);

    BswModule& operator=(const BswModule&) = default;

    QString name() const;
    std::shared_ptr<AutosarGen::module_cfg> config();
    QString ecuc_path() const;

private:
    QString ecuc_path_;
    QString bswmd_path_;
    std::shared_ptr<arx::arxml_file> bswmd_file_;
    std::shared_ptr<arx::arxml_file> ecuc_file_;
    QString name_;
    std::shared_ptr<AutosarGen::module_cfg> module_cfg_;
};