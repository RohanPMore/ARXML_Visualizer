#pragma once
#include "rapidxml/rapidxml.hpp"
#include <filesystem>
#include <vector>

class dpa_file
{
public:
    dpa_file(const std::string& filepath);
    dpa_file(const std::filesystem::path& filepath);

    const std::vector<std::filesystem::path> ecuc_files() const { return _ecuc_files; }
    const std::filesystem::path& dpa_path() { return _dpa_path; }
    const std::filesystem::path& sip_path() { return _sip_path; }
    const std::filesystem::path& ecuc_path() { return _ecuc_path; }
    const std::filesystem::path& gendata_path() { return _gendata_path; }
    const std::filesystem::path& gendata_vtt_path() { return _gendatavtt_path; }
    const std::filesystem::path& source_path() { return _source_path; }
    const std::filesystem::path& service_components_path() { return _servicecomp_path; }
    const std::filesystem::path& bsw_internal_behav_path() { return _internalbehav_path; }
    std::string_view name();
    
private:
    void _parse_subtree(rapidxml::xml_node<>* node);
    std::filesystem::path _to_local_path(std::string path);
    std::vector<char> _contents;
    std::vector<std::filesystem::path> _ecuc_files;
    rapidxml::xml_document<> _doc;
    std::filesystem::path _sip_path;
    std::filesystem::path _ecuc_path;
    std::filesystem::path _gendata_path;
    std::filesystem::path _gendatavtt_path;
    std::filesystem::path _source_path;
    std::filesystem::path _servicecomp_path;
    std::filesystem::path _internalbehav_path;
    std::filesystem::path _dpa_path;
    std::string _name;
};