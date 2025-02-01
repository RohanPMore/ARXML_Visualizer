#pragma once
#include <memory>
#include <vector>
#include <string>

#include "rapidxml/rapidxml.hpp"
#include <iostream>
#include <filesystem>

namespace arx
{

class arxml_file
{
public:
    arxml_file();
    arxml_file(const arxml_file&) = delete; // Prohibit vector reallocation.
    
    void open(const std::string& filepath);
    void open(const std::filesystem::path& filepath);
    inline char* contents() { return &(_contents[0]); }
    // void swap(arxml_file& other)
    // {
    //     _contents.swap(other._contents);
    //     _doc.swap(other._doc);

    // }

    template<int Flags>
    std::shared_ptr<rapidxml::xml_document<> > parse_xml()
    {
        _doc = std::make_shared<rapidxml::xml_document<> >();
        if(_contents.size() > 0)
            _doc->parse<Flags>(&(_contents[0]));
        return _doc;
    }

private:
    std::vector<char> _contents;
    std::shared_ptr<rapidxml::xml_document<> > _doc; //TODO: Check if this can be changed back to simple private member
};

}