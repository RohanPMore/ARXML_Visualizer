#include "bswmdfindertask.h"
#include <fstream>
#include <QDateTime>
#include <chrono>

BswmdFinderTask::BswmdFinderTask(const std::filesystem::path& sip_folder):
    sip_folder_(sip_folder)
{
    
}

void BswmdFinderTask::run()
{
    qint64 start = QDateTime::currentMSecsSinceEpoch();
    std::cout << start << ": Starting BSWMD search..." << std::endl;

    // Find BSWMD arxmls in Components/DemoComponents
    // Exclude recommended values (contain the keyword _rec)
    // Only include arxmls which contain either "bswmd" or "Copy_" in their name.
    // (PS: I can't believe there is no index.json or similar for all BSWMD files in the SIP folder?!
    //      If this does not work reliably we have to parse the contents and check for ECUC-MODULE-DEFs >:( )
    static constexpr std::array component_search_paths = {"Components", "DemoComponents"};
    static constexpr std::array bswmd_keywords = {"bswmd", "Copy_"};
    static constexpr std::array bswmd_blacklist = {"_rec"};

    bswmd_map_ = std::make_shared<bswmd_map_type>();

    // First check if there is a BSWMD folder in the root of the SIP
    const auto bswmd_path = sip_folder_/"BSWMD";
    if(std::filesystem::is_directory(bswmd_path))
    {
        for(const auto& component_folder:std::filesystem::directory_iterator(bswmd_path))
        {
            // Check if the folder is called BSWMD. Add contained arxmls to the vector
            process_bswmd_folder_(component_folder.path(), bswmd_map_);
        }
    }
    else
    {
        // Check AUTOSAR standard definitions
        const auto asr_path = sip_folder_/"DaVinciConfigurator"/"Core"/"StandardDefinition";
        if(std::filesystem::is_directory(asr_path))
        {
            process_bswmd_folder_(asr_path, bswmd_map_);
        }
        // Check component directories
        for(auto dir:component_search_paths)
        {
            // subpath = <SIP>/Components, <SIP>/DemoComponents
            const auto subpath = sip_folder_/dir;
            if(std::filesystem::is_directory(subpath))
            {
                // component_folder = <SIP>/Components/Can, <SIP>/DemoComponents/vLinkGen, ....
                for(const auto& component_folder:std::filesystem::directory_iterator(subpath))
                {
                    if(component_folder.is_directory())
                    {
                        for(const auto& component_entry:std::filesystem::directory_iterator(component_folder))
                        {
                            // Check if the folder is called BSWMD. Add contained arxmls to the vector
                            if(component_entry.path().filename() == std::string("BSWMD"))
                            {
                                process_bswmd_folder_(component_entry.path(), bswmd_map_);
                            }
                        }
                    }
                }
            }
        }
    }
    qint64 end = QDateTime::currentMSecsSinceEpoch();
    std::cout << end << ": BSWMD Search finished: " << end-start << "ms" << std::endl;
    // Publish the results.
    bswmd_map_promise_.set_value(bswmd_map_);
}

void BswmdFinderTask::process_bswmd_folder_(const std::filesystem::path& folder, std::shared_ptr<bswmd_map_type> result)
{
    static constexpr std::array bswmd_keywords = {"bswmd", "Copy_", "AUTOSAR_MOD_ECUConfigurationParameters"};
    static constexpr std::array bswmd_blacklist = {"_rec"};
    for(const auto& bswmd_entry : std::filesystem::directory_iterator(folder))
    {
        if(bswmd_entry.is_regular_file())
        {
            if(bswmd_entry.path().extension().string() == std::string(".arxml"))
            {
                bool contains_keyword = false;
                for(auto word:bswmd_keywords)
                {
                    if(bswmd_entry.path().filename().string().find(word) != std::string::npos)
                    {
                        contains_keyword = true;
                        break;
                    }
                }

                bool contains_backlist = false;
                for(auto word:bswmd_blacklist)
                {
                    if(bswmd_entry.path().filename().string().find(word) != std::string::npos)
                    {
                        contains_backlist = true;
                        break;
                    }
                }
                if(contains_keyword && !contains_backlist)
                {
                    std::vector<std::string> shortname_paths;
                    contained_module_def_(bswmd_entry.path(), shortname_paths);
                    for(auto& path: shortname_paths)
                    {
                        (*result)[path] = bswmd_entry.path();
                    }
                }
            }
        }
    }
}

void BswmdFinderTask::contained_module_def_(const std::filesystem::path& arxml_file, std::vector<std::string>& module_paths)
{
    std::ifstream file(arxml_file);
    std::error_code error;
    std::vector<char> contents;
    if(std::filesystem::is_regular_file(arxml_file) && std::filesystem::exists(arxml_file, error))
    {
        if(!error)
        {
            // Reserve enough space for the file contents (assuming UTF-8) and a termination character
            std::uintmax_t filesize = std::filesystem::file_size(arxml_file, error);
            if(!error)
            {
                contents.reserve(filesize+1U);
            }
            std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), 
                    std::back_inserter(contents));
            contents.push_back('\0');
            file.close();
        }
    }
    if(contents.size() == 0)
    {
        std::cout << "Warning: Could not open " << arxml_file.string() << "." << std::endl;
    }

    rapidxml::xml_document<> doc = rapidxml::xml_document<>();
    doc.parse<0>(&(contents[0]));
    std::vector<rapidxml::xml_node<>*> module_defs;
    find_module_def_(&doc, module_defs);

    module_paths.reserve(module_defs.size());
    for(auto def: module_defs)
    {
        std::string path = get_shortname_path_(def);
        module_paths.push_back(path);
    }
}

void BswmdFinderTask::find_module_def_(rapidxml::xml_node<>* node, std::vector<rapidxml::xml_node<>*>& module_defs)
{
    for(auto child = node->first_node(); child; child = child->next_sibling())
    {
        if(std::string("ECUC-MODULE-DEF") == child->name())
        {
            // Found module definition
            module_defs.push_back(child);
        }

        // Recurse
        find_module_def_(child, module_defs);
    }
}

std::string BswmdFinderTask::get_shortname_path_(rapidxml::xml_node<>* node)
{
    std::string result("");
    result.reserve(200);
    rapidxml::xml_node<>* current_node = node;
    while(current_node != nullptr)
    {
        for(auto child = current_node->first_node(); child; child = child->next_sibling())
        {
            if(std::string("SHORT-NAME") == child->name())
            {
                // Found shortname
                result = "/"+std::string(child->value())+result;
            }
        }
        current_node = current_node->parent();
    }
    return result;
}