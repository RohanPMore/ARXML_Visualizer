#include "param_container_val.hpp"
#include "arxml_tags.hpp"

namespace AutosarGen
{

param_container_val::param_container_val(std::shared_ptr<arx::armodel> model, const std::string& shortname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node):
    arx::referrable(model, shortname, std::move(uuid), static_cast<int>(arx::kParamContainerVal))
{
    for(auto child = node->first_node(); child; child = child->next_sibling())
    {
        if(child->name() == std::string("DEFINITION-REF"))
        {
            _def = model->get_reference(child);
        }
        else if(child->name() == std::string("PARAMETER-VALUES"))
        {
            for(auto param_child = child->first_node(); param_child; param_child = param_child->next_sibling())
            {
                if(param_child->name() == std::string("ECUC-NUMERICAL-PARAM-VALUE"))
                {
                    _num_params.emplace_back(model, param_child);
                }
                else if(param_child->name() == std::string("ECUC-TEXTUAL-PARAM-VALUE"))
                {
                    _text_params.emplace_back(model, param_child);
                }
            }
        }
        else if(child->name() == std::string("REFERENCE-VALUES"))
        {
            for(auto ref_child = child->first_node(); ref_child; ref_child = ref_child->next_sibling())
            {
                if(ref_child->name() == std::string("ECUC-REFERENCE-VALUE"))
                {
                    _refs.emplace_back(model, ref_child);
                }
            }
        }
    }
    
}

}