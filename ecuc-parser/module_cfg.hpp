#pragma once
#include "arxml_parser/referrable.hpp"
#include "rapidxml/rapidxml.hpp"
#include "arxml_parser/reference.hpp"
#include "arxml_parser/armodel.hpp"

#include <memory>

namespace AutosarGen
{

class module_cfg:public arx::referrable
{
    public:
    module_cfg(std::shared_ptr<arx::armodel> model, const std::string& shortname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node);

    inline std::shared_ptr<arx::reference> definition_ref() { return _def; }

    private:
    std::shared_ptr<arx::reference> _def;
};

}