#include "float_def.hpp"
#include "bswmd_parser.hpp"
#include "arxml_tags.hpp"
#include "arxml_parser/armodel.hpp"
#include <string>

namespace AutosarGen
{

float_def::float_def(std::shared_ptr<arx::armodel> model, const std::string& shortname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node):
    arx::referrable(model, shortname, std::move(uuid), static_cast<int>(arx::kFloatParamDef)),
    _lower_multiplicity(0),
    _upper_multiplicity(1)
{
    bswmd_parser::parse_longnames(*this, node);
    for(auto child = node->first_node(); child; child = child->next_sibling())
    {
        if(child->name() == std::string("LOWER-MULTIPLICITY"))
        {
            std::string val(child->value());
            try
            {
                _lower_multiplicity = std::stol(val);
            }
            catch(const std::exception& e)
            {
                // Keep lower_multiplicity as default
            }
        }
        else if(child->name() == std::string("UPPER-MULTIPLICITY"))
        {
            std::string val(child->value());
            try
            {
                _upper_multiplicity = std::stol(val);
            }
            catch(const std::exception& e)
            {
                // Keep upper_multiplicity as default
            }
        }
        else if(child->name() == std::string("UPPER-MULTIPLICITY-INFINITE"))
        {
            std::string val(child->value());
            if(val == "1" || val == "true" || val == "True" || val == "TRUE")
            {
                _upper_multiplicity = infinity();
            }
        }
        else if(child->name() == std::string("DEFAULT-VALUE"))
        {
            std::string val(child->value());
            if(val == "true" || val == "TRUE")
            {
                _def_val = 1;
            }
            else if(val == "false" || val == "FALSE")
            {
                _def_val = 0;
            }
            else
            {
                _def_val = std::stod(val);
            }
        }
    }
}

}
