#pragma once
#include "arxml_parser/arxml_parser.hpp"
#include "arxml_parser/armodel.hpp"
#include "rapidxml/rapidxml.hpp"

#include "module_cfg.hpp"

#include <memory>

namespace AutosarGen
{

class ecuc_parser : public arx::arxml_parser<ecuc_parser>
{
public:
    ecuc_parser(std::shared_ptr<arx::armodel> model):
        arx::arxml_parser<ecuc_parser>(model)
    {
    }

    const std::vector<std::shared_ptr<module_cfg> >& module_configs() { return module_configs_; }

    std::shared_ptr<arx::referrable> create_referrable(std::shared_ptr<arx::referrable> parent, const std::string& shortname, const std::string& tagname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node);

protected:
    std::vector<std::shared_ptr<module_cfg> > module_configs_;
};

}