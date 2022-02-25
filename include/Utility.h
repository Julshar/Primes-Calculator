#pragma once

#include <string>
#include <vector>

class Utility
{
public:
    static std::vector<std::string> split(std::string& s, char separator);
    static std::vector<std::string> split(std::string& s, char separator, char merger);
    static std::vector<std::string> split(std::string& s, char separator, std::vector<char> mergers);
    static bool isInteger(std::string& s);
};