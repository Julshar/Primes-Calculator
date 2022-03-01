#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <unordered_map>
//#include <utility>  // for std::pair

#include "Utility.h"
#include "PrimesCalcInteger.h"
#include "FileManager.h"

class UI
{
public:
    UI();
    UI(bool* terminate_flag);
    void getTask();

private:
enum class Commands
{
    HELP,
    SETTINGS,
    STOP,
    SET,
    CALCULATE,
    READ
};
enum class PropertyNames
{
    NUMBER_OF_THREADS,
    INPUT_PATH,
    OUTPUT_PATH,
    SEPARATOR,
    ASYNC_IS_DEFAULT
};
enum class CalculateParameters
{
    ASYNC,
    UPTO,
    COUNT,
    FROM,
    TO,
    SEP
};
enum class ReadParameters
{
    NUM,
    ID,
    FROM,
    TO,
    SEP
};

    static const std::unordered_map<std::string, Commands> M_COMMANDS;
    static const std::unordered_map<std::string, PropertyNames> M_PROPERTY_NAMES;
    static const std::unordered_map<std::string, CalculateParameters> M_CALCULATE_PARAMETERS;
    static const std::unordered_map<std::string, ReadParameters> M_READ_PARAMETERS;

    bool* m_stop_flag;
    bool m_terminate_flag = false;
    bool m_async_active = false;

    unsigned int m_thread_count;
    std::string m_input_path;
    std::string m_output_path;
    std::string m_separator;
    bool m_async_is_default;

    void init();
    void interpretCommand(std::string& task);

    // if property doesn't have value when read a default value should be set
    // ex. number of threads is unknown before first execute. default value would be -> std::thread::hardware_concurrency() / 2;

    void displayHelp();
    void displayProperties();

    void commandSet(std::vector<std::string>& task_separated);
    void commandCalculate(std::vector<std::string>& task_separated);
    void commandRead(std::vector<std::string>& task_separated);

    // help --- list of commands
    // settings --- list of default Properties
    // set Property_name new_value
    //
    // calculate 
    // (async)-(number_of_threads) 
    // (upto)-max_prime (count)-prime_count --- one of these has to be specified EDIT: NOT TRUE! IF NOT SPECIFIED AND ASYNC ACTIVATED CAN USE STOP COMMAND
    // (from)-(input_file) (to)-(output_file)
    // ex.
    // calculate async 5 upto 10000 from to C:/dev/c++/proj/primes.txt
    // vs
    // calculate async-5 upto-10000 from- to-C:/dev/c++/proj/primes.txt
    //
    // stop --- when async calculating this can stop and save progress
    //
    // read num-[x, y] id-[x, y] from-input_file to-output_file
    // read id "5,5000"
    //
    // NEW:
    // restore_default_settings

};