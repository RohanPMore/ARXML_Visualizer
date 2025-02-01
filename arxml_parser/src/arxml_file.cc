#include "arxml_parser/arxml_file.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>

namespace arx
{

arxml_file::arxml_file():
    _contents()
{
}

void arxml_file::open(const std::string& filepath)
{
    std::filesystem::path path(filepath);
    open(path);
}

void arxml_file::open(const std::filesystem::path& filepath)
{
    std::error_code error;
    if(std::filesystem::is_regular_file(filepath) && std::filesystem::exists(filepath, error))
    {
        if(!error)
        {
            // Reserve enough space for the file contents (assuming UTF-8) and a termination character
            std::uintmax_t filesize = std::filesystem::file_size(filepath, error);
            if(!error)
            {
                _contents.reserve(filesize+1U);
            }
            std::ifstream file(filepath);
            std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), 
                    std::back_inserter(_contents));
            _contents.push_back('\0');
            file.close();
        }
    }
}

}