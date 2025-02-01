#include "enum_literal_def.hpp"
#include "bswmd_parser.hpp"
#include "arxml_tags.hpp"
#include "arxml_parser/armodel.hpp"
#include <iostream>

namespace AutosarGen
{

enum_literal_def::enum_literal_def(std::shared_ptr<arx::armodel> model, const std::string& shortname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node):
    arx::referrable(model, shortname, std::move(uuid), static_cast<int>(arx::kEnumLiteralDef))
{
    bswmd_parser::parse_longnames(*this, node);
}

}
