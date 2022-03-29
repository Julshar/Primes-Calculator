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
    void getTask();
    bool terminate();

private:
enum class Commands
{
    HELP,
    SETTINGS,
    STOP,
    SET,
    CALCULATE,
    READ,
    EXIT,
    RESTORE
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

    static const std::string M_DEFAULT_OUTPUT_PATH;
    static const std::string M_HELP_INFO;

    bool* m_stop_flag;
    bool m_terminate_flag;
    bool m_async_active = false;

    unsigned int m_thread_count;
    std::string m_input_path;
    std::string m_output_path;
    std::string m_separator;
    bool m_async_is_default;

    void init();
    void interpretCommand(std::string& task);

    void displayHelp();
    void displayProperties();
    void restoreDefaultSettings();

    void commandSet(std::vector<std::string>& task_separated);
    void commandCalculate(std::vector<std::string>& task_separated);
    void commandRead(std::vector<std::string>& task_separated);
};