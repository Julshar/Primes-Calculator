#include "FileManager.h"

void FileManager::loadPrimes(std::string& file_path, std::vector<unsigned long long>& buffer)
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

void FileManager::savePrimes(std::string& file_path, std::string& separator, std::vector<unsigned long long>& buffer)
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

void FileManager::readRange(std::string& file_path, unsigned long long min, unsigned long long max, std::vector<unsigned long long>& buffer)
{

}

void FileManager::readIndexRange(std::string& file_path, size_t from, size_t to, std::vector<unsigned long long>& buffer)
{

}

void FileManager::convertPrimesToNumeric(std::vector<char>& byte_primes, std::vector<unsigned long long>& buffer)
{
    buffer.clear();
    buffer.push_back(2);
    std::vector<char> byte_sep = identifySeparator(byte_primes);
    size_t front = byte_sep.size() + 1;
    for(size_t back = front; back < byte_primes.size(); ++back)
    {
        if(byte_primes[back] == byte_sep[0])
        {
            unsigned long long prime = 0;
            unsigned long long multiplier = 1;
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
    unsigned long long prime = 0;
    unsigned long long multiplier = 1;
    for(size_t digit_id = byte_primes.size() - 1; digit_id >= front; --digit_id)
    {
        prime += (byte_primes[digit_id] - '0') * multiplier;
        multiplier *= 10;
    }
    buffer.push_back(prime);
}

void FileManager::convertPrimesToByte(std::vector<unsigned long long>& num_primes, std::vector<char>& buffer, std::string separator)
{
    std::vector<char> byte_sep = convertSeparatorToByte(separator);
    for(unsigned long long prime : num_primes)
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

inline std::vector<char> FileManager::convertSeparatorToByte(std::string separator)
{
    std::vector<char> byte_sep;
    for(char c : separator)
    {
        byte_sep.push_back(c);
    }
}

inline std::vector<char> FileManager::identifySeparator(std::vector<char> byte_primes)
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