#pragma once
#include "arxml_parser/referrable.hpp"
#include "rapidxml/rapidxml.hpp"
#include "bswmd_types.hpp"
#include "arxml_parser/armodel.hpp"

namespace AutosarGen
{

class symbolic_name_reference_def:public arx::referrable
{
    public:
    symbolic_name_reference_def(std::shared_ptr<arx::armodel> model, const std::string& shortname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node);
        
    multiplicity_t lower_multiplicity() { return _lower_multiplicity; }
    multiplicity_t upper_multiplicity() { return _upper_multiplicity; }

    private:
    multiplicity_t _lower_multiplicity;
    multiplicity_t _upper_multiplicity;
};

}