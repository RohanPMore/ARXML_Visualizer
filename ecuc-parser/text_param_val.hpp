#pragma once
#include "rapidxml/rapidxml.hpp"
#include "arxml_parser/reference.hpp"
#include "arxml_parser/armodel.hpp"

namespace AutosarGen
{

class text_param_val
{
public:
    text_param_val(std::shared_ptr<arx::armodel> model, rapidxml::xml_node<>* node);

    std::string_view value() { return _value; }
    std::shared_ptr<const arx::reference> definition_ref() { return std::const_pointer_cast<const arx::reference>(_def); }

private:
    std::shared_ptr<arx::reference> _def;
    std::string _value;
};

}