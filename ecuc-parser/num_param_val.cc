#include "num_param_val.hpp"
#include "param_container_val.hpp"
#include "arxml_tags.hpp"

namespace AutosarGen
{

num_param_val::num_param_val(std::shared_ptr<arx::armodel> model, rapidxml::xml_node<>* node):
    _def(nullptr)
{
    for(auto child = node->first_node(); child; child = child->next_sibling())
    {
        if(child->name() == std::string("DEFINITION-REF"))
        {
            _def = model->get_reference(child);
            // Check whether it refers to a floating point parameter or an integer parameter
            _is_floating_point = false;
            for (rapidxml::xml_attribute<> *attr = child->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(attr->name() == std::string("DEST"))
                {
                    if(attr->value() == std::string("ECUC-FLOAT-PARAM-DEF"))
                    {
                        _is_floating_point = true;
                    }
                }
            }
        }
        else if(child->name() == std::string("VALUE"))
        {
            std::string val = child->value();
            if(val == "true" || val == "TRUE")
            {
                _value_int = 1;
            }
            else if(val == "false" || val == "FALSE")
            {
                _value_int = 0;
            }
            else
            {
                if(_is_floating_point)
                {
                    try
                    {
                        _value_double = std::stod(val);
                    }
                    catch(const std::exception& e)
                    {
                        // TODO Treat error
                        _value_double = -1;
                    }
                }
                else
                {
                    try
                    {
                        _value_int = std::stoll(val);
                    }
                    catch(const std::exception& e)
                    {
                        // TODO Treat error
                        _value_int = -1;
                    }
                }
            }
        }
    }
}    

}