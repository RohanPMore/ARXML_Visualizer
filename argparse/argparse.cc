#include "argparse.hpp"

#include <string>
#include <iostream>
#include <iomanip>
#include <filesystem>

namespace AutosarGen
{

void argparse::parse(int argc, char** args)
{
    enum class parser_state
    {
        kDefault = 0,
        kOutputExpected,
        kDescriptionExpected
    };

    parser_state state = parser_state::kDefault;
    
    _executable = std::filesystem::path(args[0]).filename();

    for(int i=1;i<argc;i++)
    {
        std::string arg(args[i]);
        switch(state)
        {
            case parser_state::kDefault:
            {
                // BSWMD files
                if(arg.rfind("-B", 0) == 0)
                {
                    if(arg[2] == '"')
                    {
                        _parsed_args.bswmd_filepaths.push_back(arg.substr(3, arg.size()-4));
                    }
                    else
                    {
                         _parsed_args.bswmd_filepaths.push_back(arg.substr(2, arg.size()-2));
                    }
                }
                // ECUC files
                else if(arg.rfind("-E", 0) == 0)
                {
                    if(arg[2] == '"')
                    {
                        _parsed_args.ecuc_filepaths.push_back(arg.substr(3, arg.size()-4));
                    }
                    else
                    {
                         _parsed_args.ecuc_filepaths.push_back(arg.substr(2, arg.size()-2));
                    }
                }
                // Template files
                else if(arg.rfind("-T", 0) == 0)
                {
                    if(arg[2] == '"')
                    {
                        _parsed_args.template_filepaths.push_back(arg.substr(3, arg.size()-4));
                    }
                    else
                    {
                         _parsed_args.template_filepaths.push_back(arg.substr(2, arg.size()-2));
                    }
                }
                else if(arg == "-o")
                {
                    state = parser_state::kOutputExpected;
                }
                else if(arg == "-d")
                {
                    state = parser_state::kDescriptionExpected;
                    _parsed_args.commands.push_back(commandtype::kPrintConfigJson);
                }
                else if(arg == "-g" || arg == "--generate")
                {
                    _parsed_args.commands.push_back(commandtype::kGenerateCode);
                }
                else if(arg == "-a" || arg == "--arxml-info")
                {
                    _parsed_args.commands.push_back(commandtype::kPrintArxmlInfo);
                }
                else if(arg == "-h" || arg == "--help")
                {
                    _parsed_args.commands.push_back(commandtype::kPrintUsageInfo);
                }
                else
                {
                    // Unrecognized argument
                }
                
                break;
            }
            case parser_state::kOutputExpected:
            {
                if(arg[0] == '"')
                {
                    _parsed_args.output_dir = arg.substr(1, arg.size()-2);
                }
                else
                {
                    _parsed_args.output_dir = arg;
                }
                state = parser_state::kDefault;
                break;
            }
            case parser_state::kDescriptionExpected:
            {
                if(arg[0] == '"')
                {
                    _parsed_args.description_dir = arg.substr(1, arg.size()-2);
                }
                else
                {
                    _parsed_args.description_dir = arg;
                }
                state = parser_state::kDefault;
                break;
            }
            default:
                // Do nothing.
                break;
        }
    }
}

void argparse::print_usage()
{
    std::cout << "AUTOSAR Jinja2 Code Generator." << std::endl;
    std::cout << "Usage: " << _executable << " [Options] <Files...>"  << std::endl;
    std::cout << "Options:" << std::endl;

    auto oldflags = std::cout.flags(std::ios::left);
    std::cout << "\t" << std::setw(25) << "-h, --help" << std::setw(0)        << "Print this usage information." << std::endl;
    std::cout << "\t" << std::setw(25) << "-g, --generate" << std::setw(0)    << "Generate code files." << std::endl;
    std::cout << "\t" << std::setw(25) << "-a, --arxml-info" << std::setw(0)  << "Print the results of the ARXML parsing process." << std::endl;
    std::cout << "\t" << std::setw(25) << "-d <description_dir>" << std::setw(0) << "Generate a config description of the module configuration." << std::endl;
    std::cout << "\t" << std::setw(25) << "-o <output_dir>"  << std::setw(0)  << "Select the output directory for the generated code." << std::endl;
    (void)std::cout.flags(oldflags);

    std::cout << "Files:" << std::endl;
    std::cout << "Files can be one of the following types:" << std::endl;
    std::cout << "\t" << "- BSWMD files (Basic software module definition) in arxml format." << std::endl;
    std::cout << "\t" << "- ECUC files (ECU configuration) in arxml format." << std::endl;
    std::cout << "\t" << "- Template files in jinja2 format." << std::endl;
    std::cout << "In order to determine the type of the input file the prefixes -B (BSWMD files), -E (ECUC files), and -T (Template files) are used." << std::endl;
    std::cout << "i.e. -BAdc_bswmd.arxml means: Load the file Adc_bswmd.arxml and treat it as a BSWMD file." << std::endl;
    std::cout << "If the filepath contains a space, enclose the filepath in apostrophes (\"). i.e. -B\"C:\\Path with space\\Adc_bswmd.arxml\" ." << std::endl;
}

}