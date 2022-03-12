// #include <FileManager.h>
#include "../include/FileManager.h"

typedef unsigned long long ull;

const std::string FileManager::M_PROPERTIES_PATH = "properties.txt";

void FileManager::loadPrimes(std::string& file_path, std::vector<ull>& buffer)
{
    std::ifstream is(file_path, std::ios_base::in | std::ios_base::binary);
    if(!is)
    {
        // failed to open file
        return;
    }
    is.seekg(0, std::ios::end);
    size_t size = is.tellg();
    std::vector<char> byte_primes = std::vector<char>(size);
    is.seekg(0);
    is.read(&byte_primes[0], size);
    is.close();

    identifySeparator(byte_primes);
    convertPrimesToNumeric(byte_primes, buffer);
}

void FileManager::savePrimes(std::string& file_path, std::string& separator, std::vector<ull>& buffer)
{
    std::vector<char> byte_primes;
    convertPrimesToByte(buffer, byte_primes, separator);
    std::ofstream os(file_path, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if(!os)
    {
        // failed to open file
        return;
    }
    os.write((const char*)&byte_primes[0], byte_primes.size());
    os.close();
}

void FileManager::readRange(std::string& file_path, ull min, ull max, std::vector<ull>& buffer)
{
    std::vector<ull> all_primes;
    loadPrimes(file_path, all_primes);
    
    if(min > all_primes.back())
    {
        // warning: min prime value is larger than largest prime
        return;
    }
    bool in_range = false;
    for(size_t i = 0; i < all_primes.size(); ++i)
    {
        if(!in_range)
        {
            if(all_primes[i] >= min)
            {
                in_range = true;
            }
        }
        else
        {
            if(all_primes[i] < max)
            {
                buffer.push_back(all_primes[i]);
            }
            else
            {
                break;
            }
        }
    }
}

void FileManager::readIndexRange(std::string& file_path, size_t from, size_t to, std::vector<ull>& buffer)
{
    std::vector<ull> all_primes;
    loadPrimes(file_path, all_primes);

    if(from >= all_primes.size())
    {
        // warning: index "from" is larger than primes count
        return;
    }
    if(to > all_primes.size()) to = all_primes.size();
    for(size_t i = from; i < to; ++i)
    {
        buffer.push_back(all_primes[i]);
    }
}

std::string FileManager::readProperty(const std::string& name)
{
    std::vector<char> buffer;
    readWholeFile(M_PROPERTIES_PATH, buffer);
    size_t first_char = findProperty(buffer, name);
    if(first_char == buffer.size())
    {
        // property not found
    }
    auto last_char = std::find(buffer.begin() + first_char, buffer.end(), '\"');
    std::string value(buffer.begin() + first_char, last_char);
    return value;

    // std::string file_data(buffer.begin(), buffer.end());
    // std::vector<std::string> data_separated = Utility::split(file_data, ' ', '\"');
    // auto property_it = std::find(data_separated.begin(), data_separated.end(), name);
    // if(property_it != data_separated.end())
    // {
    //     return *(property_it + 2);
    // }
    // // No such property exist
    // return "";
}

void FileManager::setProperty(const std::string& name, const std::string& value)
{
    std::vector<char> buffer;
    readWholeFile(M_PROPERTIES_PATH, buffer);
    size_t first = findProperty(buffer, name);
    auto last = std::find(buffer.begin() + first, buffer.end(), '\"');
    buffer.erase(buffer.begin() + first, last);
    buffer.insert(buffer.begin() + first, value.begin(), value.end());
    std::ofstream os(M_PROPERTIES_PATH, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if(!os)
    {
        // failed to open file
        return;
    }
    os.write((const char*)&buffer[0], buffer.size());
    os.close();
}

void FileManager::convertPrimesToNumeric(std::vector<char>& byte_primes, std::vector<ull>& buffer)
{
    buffer.clear();
    buffer.push_back(2);
    std::vector<char> byte_sep = identifySeparator(byte_primes);
    size_t front = byte_sep.size() + 1;
    for(size_t back = front; back < byte_primes.size(); ++back)
    {
        if(byte_primes[back] == byte_sep[0])
        {
            ull prime = 0;
            ull multiplier = 1;
            for(size_t digit_id = back - 1; digit_id >= front; --digit_id)
            {
                prime += (byte_primes[digit_id] - '0') * multiplier;
                multiplier *= 10;
            }
            buffer.push_back(prime);
            back += byte_sep.size();
            front = back;
        }
    }
    ull prime = 0;
    ull multiplier = 1;
    for(size_t digit_id = byte_primes.size() - 1; digit_id >= front; --digit_id)
    {
        prime += (byte_primes[digit_id] - '0') * multiplier;
        multiplier *= 10;
    }
    buffer.push_back(prime);
}

void FileManager::convertPrimesToByte(std::vector<ull>& num_primes, std::vector<char>& buffer, std::string separator)
{
    std::vector<char> byte_sep = convertSeparatorToByte(separator);
    for(ull prime : num_primes)
    {
        std::vector<char> byte_prime;
        while(prime)
        {
            int digit = prime % 10;
            byte_prime.push_back('0' + digit);
            prime -= digit;
            prime /= 10;
        }
        buffer.insert(buffer.end(), byte_prime.rbegin(), byte_prime.rend());
        buffer.insert(buffer.end(), byte_sep.begin(), byte_sep.end());
    }
    for(char c : byte_sep) buffer.pop_back();
}

void FileManager::readWholeFile(std::string& file_path, std::vector<char>& buffer)
{
    std::ifstream is(file_path, std::ios_base::in | std::ios_base::binary);
    if(!is)
    {
        // failed to open file
        return;
    }
    is.seekg(0, is.end);
    size_t size = is.tellg();
    is.seekg(0, is.beg);
    buffer.resize(size);
    is.read(&buffer[0], size);
    is.close();
}

void FileManager::readWholeFile(const std::string& file_path, std::vector<char>& buffer)
{
    std::ifstream is(file_path, std::ios_base::in | std::ios_base::binary);
    if(!is)
    {
        // failed to open file
        return;
    }
    is.seekg(0, is.end);
    size_t size = is.tellg();
    is.seekg(0, is.beg);
    buffer.resize(size);
    is.read(&buffer[0], size);
    is.close();
}

inline std::vector<char> FileManager::convertSeparatorToByte(std::string separator)
{
    std::vector<char> byte_sep;
    for(char c : separator)
    {
        byte_sep.push_back(c);
    }
    return byte_sep;
}

inline std::vector<char> FileManager::identifySeparator(std::vector<char>& byte_primes)
{
    std::vector<char> byte_sep;
    for(int i = 1; i < byte_primes.size(); ++i)
    {
        if(byte_primes[i] < '0' || byte_primes[i] > '9')
        {
            byte_sep.push_back(byte_primes[i]);
        }
        else
        {
            break;
        }
    }
    return byte_sep;
}

// Returns index of the first character of data labeled by name. If not found returns size of input data.
size_t FileManager::findProperty(std::vector<char>& file_data, const std::string& name)
{
    size_t first = 0;
    size_t last = name.size();
    for(; last < file_data.size(); )
    {
        for(size_t i = 0; i < name.size(); ++i)
        {
            if(name[i] != file_data[first + i])
            {
                break;
            }
            else if(i == name.size() - 1)
            {
                for(; last < file_data.size(); ++last)
                {
                    if(file_data[last] == '\"') return last + 1;
                }
            }
        }
        ++first;
        ++last;
    }
    return file_data.size();
}