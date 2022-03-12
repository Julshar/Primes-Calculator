// #include <UI.h>
#include "../include/UI.h"

typedef unsigned long long ull;

const std::unordered_map<std::string, UI::Commands> UI::M_COMMANDS = 
{
    {"help", Commands::HELP},
    {"settings", Commands::SETTINGS},
    {"stop", Commands::STOP},
    {"set", Commands::SET},
    {"calculate", Commands::CALCULATE},
    {"read", Commands::READ},
    {"exit", Commands::EXIT}
};
const std::unordered_map<std::string, UI::PropertyNames> UI::M_PROPERTY_NAMES =
{
    {"number_of_threads", PropertyNames::NUMBER_OF_THREADS},
    {"input_path", PropertyNames::INPUT_PATH},
    {"output_path", PropertyNames::OUTPUT_PATH},
    {"separator", PropertyNames::SEPARATOR},
    {"async_is_default", PropertyNames::ASYNC_IS_DEFAULT}
};
const std::unordered_map<std::string, UI::CalculateParameters> UI::M_CALCULATE_PARAMETERS =
{
    {"async", CalculateParameters::ASYNC},
    {"upto", CalculateParameters::UPTO},
    {"count", CalculateParameters::COUNT},
    {"from", CalculateParameters::FROM},
    {"to", CalculateParameters::TO}
};
const std::unordered_map<std::string, UI::ReadParameters> UI::M_READ_PARAMETERS =
{
    {"num", ReadParameters::NUM},
    {"id", ReadParameters::ID},
    {"from", ReadParameters::FROM},
    {"to", ReadParameters::TO}
};

const std::string UI::M_DEFAULT_OUTPUT_PATH = "data/primes.txt";
const std::string UI::M_HELP_INFO = 
"\n"
"\n***PRIME NUMBERS CALCULATOR***"
"\n"
"\nGeneral info"
"\n\tInput syntax: \"command parameter_1 value_1 parameter_2 value_2\""
"\n\tEvery input should start with command."
"\n\tSome commands require mandatory and allow optional parameters."
"\n\tThese commands should be followed by parameters with values as the example syntax shows."
"\n\tOrder of the parameters does not matter."
"\n\tWords should be separated by spaces."
"\n\tIf a value contains space this value should be within \" signs - ex. \", \" for a separator value."
"\n\tIf user does not provide a value for a parameter program will try to use defauld value."
"\n"
"\nCOMMANDS:"
"\n"
"\nhelp - Display user manual."
"\nsettings - Display default properties."
"\nset - Set value of specified property."
"\n\tsyntax: \"set property_name value\""
"\nstop - Immediately stop calculations and save calculated prime numbers to perviously specified file (works only for async mode)."
"\ncalculate - Begin calculating prime numbers."
"\n\tparameters:"
"\n\tasync - Enable multithread mode. Value is number of threads."
"\n\tupto - Do calculations until a number specified by *value* is encountered (exclusively)."
"\n\tcount - Do calculations until *value* prime numbers have been calculated."
"\n\tfrom - Specify input file containing perviously calculated prime numbers."
"\n\tto - Specify output file where calculated prime numbers will be saved (it does not have to exist)."
"\nread - Read specified prime numbers from file."
"\n\tparameters:"
"\n\tnum - specify range of demanded prime numbers by number size: \"lowest_number, highest_number\""
"\n\tid - specify range of demanded prime numbers by index: \"lowest_index, highest_index\""
"\n\tfrom - specify input file containing prime numbers."
"\n\tto - specify output file where read prime numbers will be saved."
"\n";


UI::UI()
{
    init();
}

void UI::getTask()
{
    std::string task;
    std::getline(std::cin, task);
    interpretCommand(task);
}

bool UI::terminate()
{
    return m_terminate_flag;
}

void UI::init()
{
    m_terminate_flag = false;
    std::string read_thread_count = FileManager::readProperty("number_of_threads");
    if(read_thread_count.empty())
    {
        m_thread_count = std::thread::hardware_concurrency() / 2;
        FileManager::setProperty("number_of_threads", std::to_string(m_thread_count));
    }
    else
    {
        m_thread_count = std::stoi(read_thread_count);
    }

    m_input_path = FileManager::readProperty("input_path");

    m_output_path = FileManager::readProperty("output_path");
    if(m_output_path.empty())
    {
        m_output_path = M_DEFAULT_OUTPUT_PATH;
        FileManager::setProperty("output_path", m_output_path);
    }

    m_separator = FileManager::readProperty("separator");
    if(m_separator.empty())
    {
        m_separator = ", ";
        FileManager::setProperty("separator", m_separator);
    }

    std::string is_async = FileManager::readProperty("async_is_default");
    if(is_async == "true") m_async_is_default = true;
    else{
        m_async_is_default = false;
    }
}

void UI::interpretCommand(std::string& task)
{
    std::vector<std::string> task_separated = Utility::split(task, ' ', '\"');
    if(!task_separated.empty())
    {
        auto el = M_COMMANDS.find(task_separated[0]);
        Commands task;
        if(el != M_COMMANDS.end())
        {
            task = el->second;
        }
        else
        {
            // No such command as task_separated[0]
            return;
        }
        switch(task)
        {
        case Commands::HELP:
            displayHelp();
            break;

        case Commands::SETTINGS:
            displayProperties();
            break;

        case Commands::STOP:
            if(m_async_active)
            {
                *m_stop_flag = true;
            }
            break;

        case Commands::SET:
            commandSet(task_separated);
            break;

        case Commands::CALCULATE:
            commandCalculate(task_separated);
            break;

        case Commands::READ:
            commandRead(task_separated);
            break;

        case Commands::EXIT:
            m_terminate_flag = true;
            break;
        }
    }
}

void UI::displayHelp()
{
    std::cout << M_HELP_INFO;
}

void UI::displayProperties()
{
    std::cout << "number_of_threads : " << m_thread_count << "\n" <<
    "input_path : " << m_input_path << "\n" <<
    "output_path : " << m_output_path << "\n" <<
    "separator : " << m_separator << "\n" <<
    "async_is_default : " << m_async_is_default << "\n";
}

void UI::commandSet(std::vector<std::string>& task_separated)
{
    if(task_separated.size() == 3)
    {
        auto elem = M_PROPERTY_NAMES.find(task_separated[1]);
        PropertyNames prop;
        if(elem != M_PROPERTY_NAMES.end())
        {
            prop = elem->second;
        }
        else
        {
            // task_separated[1] is an Invalid property name
            return;
        }
        switch(prop)
        {
        case PropertyNames::ASYNC_IS_DEFAULT:
            if(task_separated[2] == "1") task_separated[2] = "true";
            else if(task_separated[2] == "0") task_separated[2] = "false";

            if(task_separated[2] == "true") m_async_is_default = true;
            else if(task_separated[2] == "false") m_async_is_default = false;
            else
            {
                // invalid value
                return;
            }
            break;

        case PropertyNames::INPUT_PATH:
            m_input_path = task_separated[2];
            break;

        case PropertyNames::NUMBER_OF_THREADS:
            if(Utility::isInteger(task_separated[2]) == false)
            {
                // invalid value
                return;
            }
            break;

        case PropertyNames::OUTPUT_PATH:
            m_output_path = task_separated[2];
            break;

        case PropertyNames::SEPARATOR:
            m_separator = task_separated[2];
            break;
        }
        FileManager::setProperty(task_separated[1], task_separated[2]);
    }
    else
    {
        // invalid number of arguments for set command
    }
}

void UI::commandCalculate(std::vector<std::string>& task_separated)
{
    bool async = m_async_is_default;
    unsigned int thread_count = m_thread_count;
    std::pair<bool, std::string> input_file = {false, m_input_path};
    std::string output_path = m_output_path;
    std::string separator = m_separator;
    std::pair<bool, ull> number_cap = {false, 0};
    std::pair<bool, unsigned int> count_cap = {false, 0};

    for(size_t i = 1; i < task_separated.size(); ++i)
    {
        auto elem = M_CALCULATE_PARAMETERS.find(task_separated[i]);
        CalculateParameters param;
        if(elem != M_CALCULATE_PARAMETERS.end())
        {
            param = elem->second;
        }
        else
        {
            // task_separated[i] is invalid parameter name
            return;
        }
        switch(param)   // have to catch error: i + 1 can go out of task_separated bounds
        {
        case CalculateParameters::ASYNC:
            async = true;
            if(Utility::isInteger(task_separated[i + 1]))
            {
                thread_count = std::stoi(task_separated[i + 1]);
                ++i;
            }
            break;
        case CalculateParameters::UPTO:
            if(Utility::isInteger(task_separated[i + 1]))
            {
                number_cap.first = true;
                number_cap.second = std::stoi(task_separated[i + 1]);
                ++i;
            }
            else
            {
                // invalid argument for separated[i] parameter
                // no need to return coz of default behaviours
            }
            break;
        case CalculateParameters::COUNT:
            if(Utility::isInteger(task_separated[i + 1]))
            {
                count_cap.first = true;
                count_cap.second = std::stoi(task_separated[i + 1]);
                ++i;
            }
            else
            {
                // invalid argument for separated[i] parameter
                // no need to return coz of default behaviours
            }
            break;
        case CalculateParameters::FROM:
            input_file.first = true;

            // if no argument after from parameter then default should be used
            input_file.second = task_separated[i + 1];
            ++i;
            break;
        case CalculateParameters::TO:
            output_path = task_separated[i + 1];
            ++i;
            break;
        case CalculateParameters::SEP:
            separator = task_separated[i + 1];
            ++i;
            break;
        }
    }
    PrimesCalcInteger calc;
    if(input_file.first)
    {
        calc.init(m_stop_flag, separator, output_path, input_file.second);
    }
    else
    {
        calc.init(m_stop_flag, separator, output_path);
    }

    if(async)
    {
        m_async_active = true;
        if(number_cap.first)
        {
            calc.calcAsyncUpto(thread_count, number_cap.second);
        }
        else if(count_cap.first)
        {
            calc.calcAsyncCount(thread_count, count_cap.second);
        }
        else
        {
            calc.calcAsyncUpto(thread_count);
        }
        m_async_active = false;
    }   
    else
    {
        if(number_cap.first)
        {
            calc.calcUpto(number_cap.second);
        }
        else if(count_cap.first)
        {
            calc.calcCount(count_cap.second);
        }
        else
        {
            calc.calcUpto();
        }
    } 
    calc.saveToFile();
}

void UI::commandRead(std::vector<std::string>& task_separated)
{
    std::string input_path = m_input_path;
    std::string separator = m_separator;
    std::pair<bool, std::string> output = {false, ""};
    std::pair<bool, unsigned int> single_index = {false, 0};
    std::pair<bool, std::pair<unsigned int, unsigned int>> index_range = {false, {0, 0}};
    std::pair<bool, std::pair<ull, ull>> num_range = {false, {0, 0}};

    for(size_t i = 1; i < task_separated.size(); ++i)
    {
        auto elem = M_READ_PARAMETERS.find(task_separated[i]);
        ReadParameters param;
        if(elem != M_READ_PARAMETERS.end())
        {
            param = elem->second;
        }
        else
        {
            // task_separated[i] is invalid parameter name
            return;
        }
        std::vector<std::string> range;
        switch(param)   // have to catch error: i + 1 can go out of task_separated bounds
        {
        case ReadParameters::FROM:
            input_path = task_separated[i + 1];
            ++i;
            break;
        case ReadParameters::TO:
            output.first = true;
            output.second = task_separated[i + 1];
            ++i;
            break;
        case ReadParameters::ID:
            range = Utility::split(task_separated[i + 1], ',');
            if(range.size() == 1)
            {
                single_index.first = true;
                single_index.second = std::stoi(range[0]);
                ++i;
            }
            else if(range.size() == 2)
            {
                index_range.first = true;
                index_range.second.first = std::stoi(range[0]);
                index_range.second.second = std::stoi(range[1]);
                ++i;
            }
            else
            {
                // too many arguments (more than 2)
                // so ex. [4, 50, 30]
                return;
            }
            break;
        case ReadParameters::NUM:
            range = Utility::split(task_separated[i + 1], ',');
            if(range.size() == 2)
            {
                num_range.first = true;
                num_range.second.first = std::stoi(range[0]);
                num_range.second.second = std::stoi(range[1]);
                ++i;
            }
            else
            {
                // num argument needs range described by exactly 2 numbers
                return;
            }
            break;
        case ReadParameters::SEP:
            separator = task_separated[i + 1];
            ++i;
            break;
        }
    }
    std::vector<ull> buffer;
    if(single_index.first)
    {
        FileManager::readIndexRange(input_path, single_index.second, single_index.second + 1, buffer);
    }
    else if(index_range.first)
    {
        FileManager::readIndexRange(input_path, index_range.second.first, index_range.second.second + 1, buffer);
    }
    else if(num_range.first)
    {
        FileManager::readRange(input_path, num_range.second.first, num_range.second.second + 1, buffer);
    }
    else
    {
        // no range was specified
        return;
    }

    if(output.first)
    {
        FileManager::savePrimes(output.second, separator, buffer);
    }
    else
    {
        // Print all numbers from buffer into console
        // use separator
    }
}
