#pragma once

#include <string>
#include <vector>

class Utility
{
public:
    static std::vector<std::string> split(std::string& s, char separator);
    static std::vector<std::string> split(std::string& s, char separator, char merger);
    static bool isInteger(std::string& s);
};