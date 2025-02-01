#pragma once
#include "rapidxml/rapidxml.hpp"
#include "arxml_parser/reference.hpp"
#include "arxml_parser/armodel.hpp"

namespace AutosarGen
{


class reference_val
{
public:
    reference_val(std::shared_ptr<arx::armodel> model, rapidxml::xml_node<>* node);

    std::shared_ptr<const arx::reference> value() { return std::const_pointer_cast<const arx::reference>(_value); }
    std::shared_ptr<const arx::reference> definition_ref() const { return std::const_pointer_cast<const arx::reference>(_def); }

private:
    std::shared_ptr<arx::reference> _def;
    std::shared_ptr<arx::reference> _value;
};

}