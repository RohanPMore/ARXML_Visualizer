#include "text_param_val.hpp"
#include "arxml_tags.hpp"

namespace AutosarGen
{

text_param_val::text_param_val(std::shared_ptr<arx::armodel> model, rapidxml::xml_node<>* node):
    _def(nullptr)
{
    for(auto child = node->first_node(); child; child = child->next_sibling())
    {
        if(child->name() == std::string("DEFINITION-REF"))
        {
            _def = model->get_reference(child);
        }
        else if(child->name() == std::string("VALUE"))
        {
            _value = child->value();
        }
    }
}    

}