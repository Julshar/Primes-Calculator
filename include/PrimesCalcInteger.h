#pragma once

#include <string>
#include <limits>
#include <vector>
#include <thread>
#include <mutex>

#include "FileManager.h"

class PrimesCalcInteger
{
public:
    PrimesCalcInteger(std::string separator, std::string output_file, std::string input_file);
    PrimesCalcInteger();
    void init(std::string separator, std::string output_file, std::string input_file);
    void init(std::string separator, std::string output_file);

    void calcUpto(unsigned long long max = MAX_ULL);
    void calcCount(unsigned int count);
    void calcAsyncUpto(unsigned int thread_count, unsigned long long max = MAX_ULL);
    void calcAsyncCount(unsigned int thread_count, unsigned int count);

private:
    static constexpr unsigned long long MAX_ULL = ULLONG_MAX - 1000;

    std::string m_separator;
    std::string m_output_file;
    bool* m_stop_flag;

    std::vector<unsigned long long> m_primes;
    std::mutex m_primes_lock;
    unsigned long long m_current_number;
    std::mutex m_current_number_lock;

    bool checkIfIsPrime(unsigned long long number, std::vector<unsigned long long>& primes);
};