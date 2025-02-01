#include "bswmd_parser.hpp"
#include "module_def.hpp"
#include "param_container_def.hpp"
#include "enum_def.hpp"
#include "enum_literal_def.hpp"
#include "int_def.hpp"
#include "float_def.hpp"
#include "bool_def.hpp"
#include "reference_def.hpp"
#include "choice_reference_def.hpp"
#include "choice_container_def.hpp"
#include "function_name_def.hpp"
#include "symbolic_name_reference_def.hpp"
#include "string_def.hpp"

namespace AutosarGen
{
    
std::shared_ptr<arx::referrable> bswmd_parser::create_referrable(std::shared_ptr<arx::referrable> parent, const std::string& shortname, const std::string& tagname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node)
{
    std::shared_ptr<arx::referrable> ref;
    if(tagname == "ECUC-MODULE-DEF")
    {
        ref = std::make_shared<module_def>(_model, shortname, std::move(uuid), node);
    }
    else if(tagname == "ECUC-PARAM-CONF-CONTAINER-DEF")
    {
        ref =  std::make_shared<param_container_def>(_model, shortname, std::move(uuid), node);
    }
    else if(tagname == "ECUC-ENUMERATION-PARAM-DEF")
    {
        ref =  std::make_shared<enum_def>(_model, shortname, std::move(uuid), node);
    }
    else if(tagname == "ECUC-ENUMERATION-LITERAL-DEF")
    {
        ref =  std::make_shared<enum_literal_def>(_model, shortname, std::move(uuid), node);
    }
    else if(tagname == "ECUC-INTEGER-PARAM-DEF")
    {
        ref =  std::make_shared<int_def>(_model, shortname, std::move(uuid), node);
    }
    else if(tagname == "ECUC-FLOAT-PARAM-DEF")
    {
        ref =  std::make_shared<float_def>(_model, shortname, std::move(uuid), node);
    }
    else if(tagname == "ECUC-BOOLEAN-PARAM-DEF")
    {
        ref =  std::make_shared<bool_def>(_model, shortname, std::move(uuid), node);
    }
    else if(tagname == "ECUC-REFERENCE-DEF")
    {
        ref =  std::make_shared<reference_def>(_model, shortname, std::move(uuid), node);
    }
    else if(tagname == "ECUC-CHOICE-REFERENCE-DEF")
    {
        ref =  std::make_shared<choice_reference_def>(_model, shortname, std::move(uuid), node);
    }
    else if(tagname == "ECUC-CHOICE-CONTAINER-DEF")
    {
        ref =  std::make_shared<choice_container_def>(_model, shortname, std::move(uuid), node);
    }
    else if(tagname == "ECUC-FUNCTION-NAME-DEF")
    {
        ref =  std::make_shared<function_name_def>(_model, shortname, std::move(uuid), node);
    }
    else if(tagname == "ECUC-SYMBOLIC-NAME-REFERENCE-DEF")
    {
        ref =  std::make_shared<symbolic_name_reference_def>(_model, shortname, std::move(uuid), node);
    }
    else if(tagname == "ECUC-STRING-PARAM-DEF")
    {
        ref =  std::make_shared<function_name_def>(_model, shortname, std::move(uuid), node);
    }
    else
    {
        // Use default handler.
        ref =  arx::arxml_parser<bswmd_parser>::create_default_referrable(parent, shortname, tagname, std::move(uuid), node);
    }
    
    return ref;
}

}