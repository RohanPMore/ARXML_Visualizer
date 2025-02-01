#pragma once
#include "arxml_parser/arxml_parser.hpp"

namespace AutosarGen
{

class bswmd_parser: public arx::arxml_parser<bswmd_parser>
{
    public:
    bswmd_parser(std::shared_ptr<arx::armodel> model):
        arx::arxml_parser<bswmd_parser>(model)
    {
    }

    std::shared_ptr<arx::referrable> create_referrable(std::shared_ptr<arx::referrable> parent, const std::string& shortname, const std::string& tagname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node);
};

}