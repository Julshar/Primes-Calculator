#include "UI.h"

const std::string UI::M_PROPERTIES_PATH = "properties.txt";
const std::unordered_map<std::string, UI::Commands> UI::M_COMMANDS = 
{
    {"help", Commands::HELP},
    {"settings", Commands::SETTINGS},
    {"stop", Commands::STOP},
    {"set", Commands::SET},
    {"calculate", Commands::CALCULATE},
    {"read", Commands::READ}
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

UI::UI()
{
    m_thread_count = std::thread::hardware_concurrency() / 2;
    m_separator = ", ";

}

void UI::getTask()
{
    std::getline(std::cin, m_task);
    interpretCommand();
}

void UI::interpretCommand()
{
    std::vector<std::string> separated = Utility::split(m_task, ' ', '\"');
    if(!separated.empty())
    {
        auto el = M_COMMANDS.find(separated[0]);
        Commands task;
        if(el != M_COMMANDS.end())
        {
            task = el->second;
        }
        else
        {
            // No such command as separated[0]
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
            // stop calculations if currently working and if async mode active
            break;

        case Commands::SET:
            if(separated.size() == 3)
            {
                auto elem = M_PROPERTY_NAMES.find(separated[1]);
                PropertyNames prop;
                if(elem != M_PROPERTY_NAMES.end())
                {
                    prop = elem->second;
                }
                else
                {
                    // separated[1] is an Invalid property name
                    return;
                }
                switch(prop)
                {
                case PropertyNames::ASYNC_IS_DEFAULT:
                    if(separated[2] == "true" || separated[2] == "false") break;
                    else if(separated[2] == "1") separated[2] = "true";
                    else if(separated[2] == "0") separated[2] = "false";
                    else
                    {
                        // invalid value
                        return;
                    }
                    break;

                case PropertyNames::INPUT_PATH:
                    break;

                case PropertyNames::NUMBER_OF_THREADS:
                    if(Utility::isInteger(separated[2]) == false)
                    {
                        // invalid value
                        return;
                    }
                    break;

                case PropertyNames::OUTPUT_PATH:
                    break;

                case PropertyNames::SEPARATOR:
                    break;
                }
            }
            else
            {
                // invalid number of arguments for set command
            }
            break;

        case Commands::CALCULATE:
            bool async = m_async_is_default;
            unsigned int thread_count = m_thread_count;

            //std::string input_path;
            //std::string output_path;
            //std::string separator;

            for(size_t i = 1; i < separated.size(); ++i)
            {
                auto elem = M_CALCULATE_PARAMETERS.find(separated[i]);
                CalculateParameters param;
                if(elem != M_CALCULATE_PARAMETERS.end())
                {
                    param = elem->second;
                }
                else
                {
                    // separated[i] is invalid parameter name
                    return;
                }
                switch(param)
                {
                case CalculateParameters::ASYNC:
                    async = true;
                    if(Utility::isInteger(separated[i + 1]))
                    {
                        thread_count = std::stoi(separated[i + 1]);
                        ++i;
                    }
                    break;
                case CalculateParameters::UPTO:
                    break;
                case CalculateParameters::COUNT:
                    break;
                case CalculateParameters::FROM:
                    break;
                case CalculateParameters::TO:
                    break;
                }
            }
            // initiate calculations
            break;

        case Commands::READ:
            break;
        }
    }
}

void UI::setProperty(std::string name, std::string value)
{

}

void UI::readProperty(std::string name)
{
    
}

void UI::displayHelp()
{

}

void UI::displayProperties()
{

}