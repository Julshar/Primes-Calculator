#pragma once

#include <string>
#include <vector>
#include <fstream>
// #include <algorithm> // find

#include "Utility.h"

class FileManager
{
public:
    static void loadPrimes(std::string& file_path, std::vector<unsigned long long>& buffer);
    static void savePrimes(std::string& file_path, std::string& separator, std::vector<unsigned long long>& buffer);

    static void readRange(std::string& file_path, unsigned long long min, unsigned long long max, std::vector<unsigned long long>& buffer);
    static void readIndexRange(std::string& file_path, size_t from, size_t to, std::vector<unsigned long long>& buffer);

    static std::string readProperty(const std::string& name);
    static void setProperty(const std::string& name, const std::string& value);

private:
    static const std::string M_PROPERTIES_PATH;

    static void convertPrimesToNumeric(std::vector<char>& byte_primes, std::vector<unsigned long long>& buffer);
    static void convertPrimesToByte(std::vector<unsigned long long>& num_primes, std::vector<char>& buffer, std::string separator);
    static void readWholeFile(std::string& file_path, std::vector<char>& buffer);
    static void readWholeFile(const std::string& file_path, std::vector<char>& buffer);
    static std::vector<char> convertSeparatorToByte(std::string separator);
    static std::vector<char> identifySeparator(std::vector<char>& byte_primes);
    static size_t findProperty(std::vector<char>& file_data, const std::string& name);
};