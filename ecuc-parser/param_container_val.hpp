#pragma once
#include "arxml_parser/referrable.hpp"
#include "rapidxml/rapidxml.hpp"
#include "arxml_parser/reference.hpp"
#include "arxml_parser/armodel.hpp"

#include "num_param_val.hpp"
#include "text_param_val.hpp"
#include "reference_val.hpp"

namespace AutosarGen
{


class param_container_val:public arx::referrable
{
public:
    param_container_val(std::shared_ptr<arx::armodel> model, const std::string& shortname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node);

    std::vector<num_param_val>& numerical_params() { return _num_params; }
    std::vector<text_param_val>& textual_params() { return _text_params; }
    std::vector<reference_val>& references() { return _refs; }
    std::shared_ptr<arx::reference> definition_ref() { return _def; }

private:
    std::shared_ptr<arx::reference> _def;
    std::vector<num_param_val> _num_params;
    std::vector<text_param_val> _text_params;
    std::vector<reference_val> _refs;

};

}