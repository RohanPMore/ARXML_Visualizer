#pragma once

#include <memory>
#include <vector>
#include <string>

namespace AutosarGen
{

enum class commandtype
{
    kPrintUsageInfo = 0,
    kPrintArxmlInfo,
    kPrintConfigJson,
    kGenerateCode
};

struct parsed_arguments
{
    std::vector<std::string> bswmd_filepaths;
    std::vector<std::string> ecuc_filepaths;
    std::vector<std::string> template_filepaths;
    std::string output_dir;
    std::string description_dir;
    std::vector<commandtype> commands;
};

class argparse
{

public:
    void parse(int num_args, char** args);
    void print_usage();

    const parsed_arguments& args() { return _parsed_args; }
private:
    parsed_arguments _parsed_args;
    std::string _executable;
};

}