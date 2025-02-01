#include "symbolic_name_reference_def.hpp"
#include "bswmd_parser.hpp"
#include "arxml_tags.hpp"
#include "arxml_parser/armodel.hpp"

namespace AutosarGen
{

symbolic_name_reference_def::symbolic_name_reference_def(std::shared_ptr<arx::armodel> model, const std::string& shortname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node):
    arx::referrable(model, shortname, std::move(uuid), static_cast<int>(arx::kSymbolicNameReferenceDef)),
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
    }
}

}
