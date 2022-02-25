#pragma once

#include <string>
#include <limits>
#include <vector>
#include <thread>
#include <mutex>

class PrimesCalcInteger
{
public:
    PrimesCalcInteger(std::string separator, std::string output_file, std::string input_file = "primes.txt");
    PrimesCalcInteger();

    void CalcUpto(unsigned long long max = MAX_ULL);
    void CalcCount(unsigned int count);
    void CalcAsyncUpto(unsigned int thread_count, unsigned long long max = MAX_ULL);
    void CalcAsyncCount(unsigned int thread_count, unsigned int count);

private:
    static constexpr unsigned long long MAX_ULL = ULLONG_MAX - 1000;

    std::vector<char> m_separator;
    std::vector<unsigned long long> m_primes;
    std::mutex m_primes_lock;
    unsigned long long m_current_number;
    std::mutex m_current_number_lock;
};