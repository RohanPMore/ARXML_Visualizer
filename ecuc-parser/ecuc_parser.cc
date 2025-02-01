#include "ecuc_parser.hpp"
#include "param_container_val.hpp"
#include "num_param_val.hpp"

namespace AutosarGen
{

std::shared_ptr<arx::referrable> ecuc_parser::create_referrable(std::shared_ptr<arx::referrable> parent, const std::string& shortname, const std::string& tagname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node)
{
    std::shared_ptr<arx::referrable> ref;
    if(tagname == "ECUC-MODULE-CONFIGURATION-VALUES")
    {
        ref = std::make_shared<module_cfg>(_model, shortname, std::move(uuid), node);
        module_configs_.push_back(std::static_pointer_cast<module_cfg>(ref));
    }
    else if(tagname == "ECUC-CONTAINER-VALUE")
    {
        ref = std::make_shared<param_container_val>(_model, shortname, std::move(uuid), node);
    }
    else
    {
        // Call default handler
        ref = arxml_parser<ecuc_parser>::create_default_referrable(parent, shortname, tagname, std::move(uuid), node);
    }

    return ref;    
}

}