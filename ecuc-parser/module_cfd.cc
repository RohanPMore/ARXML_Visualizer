#include "module_cfg.hpp"
#include "arxml_tags.hpp"

namespace AutosarGen
{

module_cfg::module_cfg(std::shared_ptr<arx::armodel> model, const std::string& shortname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node):
    arx::referrable(model, shortname, std::move(uuid), static_cast<int>(arx::kModuleCfg)),
    _def(nullptr)
{
    for(auto child = node->first_node(); child; child = child->next_sibling())
    {
        if(child->name() == std::string("DEFINITION-REF"))
        {
            _def = model->get_reference(child);
        }
    }
}    

}