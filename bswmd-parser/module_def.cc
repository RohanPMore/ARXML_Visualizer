#include "module_def.hpp"
#include "bswmd_parser.hpp"
#include "arxml_tags.hpp"
#include "arxml_parser/armodel.hpp"

namespace AutosarGen
{

module_def::module_def(std::shared_ptr<arx::armodel> model, const std::string& shortname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node):
    arx::referrable(model, shortname, std::move(uuid), static_cast<int>(arx::kModuleDef))
{
    
}

}
