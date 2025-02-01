#pragma once
#include "arxml_parser/referrable.hpp"
#include "rapidxml/rapidxml.hpp"

namespace AutosarGen
{

class arx::armodel;

class enum_literal_def:public arx::referrable
{
    public:
    enum_literal_def(std::shared_ptr<arx::armodel> model, const std::string& shortname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node);
};

}