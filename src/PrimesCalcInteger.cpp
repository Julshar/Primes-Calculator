#include "PrimesCalcInteger.h"

typedef unsigned long long ull;

PrimesCalcInteger::PrimesCalcInteger(std::string separator, std::string output_file, std::string input_file)
{
    init(separator, output_file, input_file);
}

PrimesCalcInteger::PrimesCalcInteger()
{
    init(", ", "primes");
}

void PrimesCalcInteger::init(std::string separator, std::string output_file, std::string input_file)
{
    m_separator = separator;
    m_output_file = output_file;

    m_primes = std::vector<ull>{};
    FileManager::loadPrimes(input_file, m_primes);
    m_current_number = m_primes.back() + 2;
}

void PrimesCalcInteger::init(std::string separator, std::string output_file)
{
    m_separator = separator;
    m_output_file = output_file;

    m_primes = std::vector<ull>{2, 3};
    m_current_number = {5};
}

void PrimesCalcInteger::calcUpto(unsigned long long max = MAX_ULL)
{

}

void PrimesCalcInteger::calcCount(unsigned int count)
{

}

void PrimesCalcInteger::calcAsyncUpto(unsigned int thread_count, unsigned long long max = MAX_ULL)
{
    std::vector<std::thread> threadpool;
    for(int i = 0; i < thread_count; ++i)
    {
        threadpool.push_back(std::thread([this, &max](){
            PrimesCalcInteger::m_current_number_lock.lock();
            ull current_number = PrimesCalcInteger::m_current_number;
            PrimesCalcInteger::m_current_number += 2;
            PrimesCalcInteger::m_current_number_lock.unlock();

            PrimesCalcInteger::m_primes_lock.lock();
            std::vector<ull> low_primes = PrimesCalcInteger::m_primes;
            PrimesCalcInteger::m_primes_lock.unlock();

            while(!PrimesCalcInteger::m_stop_flag && current_number < max)
            {
                if(low_primes.back() * low_primes.back() <= current_number)
                {
                    PrimesCalcInteger::m_primes_lock.lock();
                    std::vector<ull> low_primes = PrimesCalcInteger::m_primes;
                    PrimesCalcInteger::m_primes_lock.unlock();
                }
                if(PrimesCalcInteger::checkIfIsPrime(current_number, low_primes))
                {
                    PrimesCalcInteger::m_primes_lock.lock();
                    size_t last = m_primes.size() - 1;
                    while(current_number < PrimesCalcInteger::m_primes[last]) --last;
                    PrimesCalcInteger::m_primes.insert(PrimesCalcInteger::m_primes.begin() + last + 1, current_number);
                    PrimesCalcInteger::m_primes_lock.unlock();
                }
                PrimesCalcInteger::m_current_number_lock.lock();
                current_number = PrimesCalcInteger::m_current_number;
                PrimesCalcInteger::m_current_number += 2;
                PrimesCalcInteger::m_current_number_lock.unlock();
            }
        }));
    }

    for(std::thread& t : threadpool)
    {
        t.join();
    }
}

void PrimesCalcInteger::calcAsyncCount(unsigned int thread_count, unsigned int count)
{
    
}

inline bool PrimesCalcInteger::checkIfIsPrime(unsigned long long number, std::vector<unsigned long long>& primes)
{
    ull* curr_prime = &primes.front() + 1;
    while((*curr_prime) * (*curr_prime) <= number)
    {
        if(number % (*curr_prime) == 0)
        {
            return false;
        }
        ++curr_prime;
    }
    return true;
}