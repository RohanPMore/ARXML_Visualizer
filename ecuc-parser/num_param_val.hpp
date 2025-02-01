#pragma once
#include "rapidxml/rapidxml.hpp"
#include "arxml_parser/reference.hpp"
#include "arxml_parser/armodel.hpp"

#include <memory>

namespace AutosarGen
{

class num_param_val
{
public:
    num_param_val(std::shared_ptr<arx::armodel> model, rapidxml::xml_node<>* node);

    int64_t value()
    {
        if(!_is_floating_point)
        {
            return _value_int; 
        }
        else
        {
            return static_cast<int64_t>(_value_double); 
        }
    }

    double value_floating()
    {
        if(_is_floating_point)
        {
            return _value_double; 
        }
        else
        {
            return static_cast<double>(_value_int); 
        }
    }
    std::shared_ptr<const arx::reference> definition_ref() { return std::const_pointer_cast<const arx::reference>(_def); }
    
private:
    std::shared_ptr<arx::reference> _def;
    int64_t _value_int;
    double _value_double;
    bool _is_floating_point;
};

}