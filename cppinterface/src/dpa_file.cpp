#include "dpa_file.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <locale>
#include <codecvt>

#include <boost/algorithm/string.hpp>

dpa_file::dpa_file(const std::filesystem::path& filepath):
    _dpa_path(filepath)
{
    // DEBUG
    std::cout << "Parsing DPA: " << filepath << std::endl;

    // Set the name based on the filename
    _name = filepath.stem().string();

    // Load DPA contents from file
    std::ifstream file(filepath);
    _contents = std::vector<char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    _contents.push_back('\0');
    file.close();

    // Parse
    _doc.parse<0>(&(_contents[0]));

    // Determine values of interest
    dpa_file::_parse_subtree(&_doc);
}

dpa_file::dpa_file(const std::string& filepath):
    dpa_file(std::filesystem::path(filepath))
{
}


void dpa_file::_parse_subtree(rapidxml::xml_node<>* node)
{
    if(std::string("Folders") == node->name())
    {
        // We have reached the "folders" node. Extract the required information and do not recurse further.
        for(auto child = node->first_node(); child; child = child->next_sibling())
        {
            if(std::string("SIP") == child->name())
            {
                _sip_path = std::filesystem::weakly_canonical(_dpa_path.parent_path()/_to_local_path(child->value()));
                // DEBUG
                std::cout << "Found SIP path: " << _sip_path.u8string() << std::endl;
            }
            else if(std::string("ECUC") == child->name())
            {
                _ecuc_path = std::filesystem::weakly_canonical(_dpa_path.parent_path()/_to_local_path(child->value()));
                // DEBUG
                std::cout << "Found ECUC path: " << _ecuc_path.u8string() << std::endl;
            }
            else if(std::string("GenData") == child->name())
            {
                _gendata_path = std::filesystem::weakly_canonical(_dpa_path.parent_path()/_to_local_path(child->value()));
                // DEBUG
                std::cout << "Found GenData path: " << _gendata_path.u8string() << std::endl;
            }
            else if(std::string("GenDataVtt") == child->name())
            {
                _gendatavtt_path = std::filesystem::weakly_canonical(_dpa_path.parent_path()/_to_local_path(child->value()));
                // DEBUG
                std::cout << "Found GenDataVtt path: " << _gendatavtt_path.u8string() << std::endl;
            }
            else if(std::string("Source") == child->name())
            {
                _source_path = std::filesystem::weakly_canonical(_dpa_path.parent_path()/_to_local_path(child->value()));
                // DEBUG
                std::cout << "Found Source path: " << _source_path.u8string() << std::endl;
            }
            else if(std::string("ServiceComponents") == child->name())
            {
                _servicecomp_path = std::filesystem::weakly_canonical(_dpa_path.parent_path()/_to_local_path(child->value()));
                // DEBUG
                std::cout << "Found Service Components path: " << _servicecomp_path.u8string() << std::endl;
            }
            else if(std::string("BswInternalBehaviour") == child->name())
            {
                _internalbehav_path = std::filesystem::weakly_canonical(_dpa_path.parent_path()/_to_local_path(child->value()));
                // DEBUG
                std::cout << "Found BSW Internal Behaviour path: " << _internalbehav_path.u8string() << std::endl;
            }
        }
    }
    else if(std::string("EcucSplitter") == node->name())
    {
        // Iterate over referenced ECUC files.
        for(auto child = node->first_node(); child; child = child->next_sibling())
        {
            if(std::string("Splitter") == child->name())
            {
                for(auto attr = child->first_attribute(); attr; attr = attr->next_attribute())
                {
                    if(std::string("File") == attr->name())
                    {
                        _ecuc_files.push_back(std::filesystem::weakly_canonical(_dpa_path.parent_path()/_to_local_path(attr->value())));
                        break;
                    }
                }
                
            }
        }
    }
    else
    {
        // Recurse
        for(auto child = node->first_node(); child; child = child->next_sibling())
        {
            _parse_subtree(child);
        }
    }
}

std::filesystem::path dpa_file::_to_local_path(std::string path)
{
    std::replace(path.begin(), path.end(), '\\', static_cast<char>(std::filesystem::path::preferred_separator));
    return std::filesystem::path(path);
}

std::string_view dpa_file::name()
{
    return _name;
}