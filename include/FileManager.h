#pragma once

#include <string>
#include <vector>
#include <fstream>

class FileManager
{
public:
    static void loadPrimes(std::string& file_path, std::vector<unsigned long long>& buffer);
    static void savePrimes(std::string& file_path, std::string& separator, std::vector<unsigned long long>& buffer);

    static void readRange(std::string& file_path, unsigned long long min, unsigned long long max, std::vector<unsigned long long>& buffer);
    static void readIndexRange(std::string& file_path, size_t from, size_t to, std::vector<unsigned long long>& buffer);

private:
    static void convertPrimesToNumeric(std::vector<char>& byte_primes, std::vector<unsigned long long>& buffer);
    static void convertPrimesToByte(std::vector<unsigned long long>& num_primes, std::vector<char>& buffer, std::string separator);
    static std::vector<char> convertSeparatorToByte(std::string separator);
    static std::vector<char> identifySeparator(std::vector<char> byte_primes);
};