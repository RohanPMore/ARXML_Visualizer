#pragma once
#include "arxml_parser/referrable.hpp"
#include "rapidxml/rapidxml.hpp"

namespace AutosarGen
{

// Forward declaration
class arx::armodel;

class module_def:public arx::referrable
{
    public:
    module_def(std::shared_ptr<arx::armodel> model, const std::string& shortname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node);
};

}