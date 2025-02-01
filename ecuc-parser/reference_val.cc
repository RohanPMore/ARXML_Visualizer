#include "reference_val.hpp"
#include "arxml_tags.hpp"

namespace AutosarGen
{

reference_val::reference_val(std::shared_ptr<arx::armodel> model, rapidxml::xml_node<>* node):
    _def(nullptr),
    _value(nullptr)
{
    for(auto child = node->first_node(); child; child = child->next_sibling())
    {
        if(child->name() == std::string("DEFINITION-REF"))
        {
            _def = model->get_reference(child);
        }
        else if(child->name() == std::string("VALUE-REF"))
        {
            _value = model->get_reference(child);
        }
    }
}    

}