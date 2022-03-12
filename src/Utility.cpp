// #include "Utility.h"
#include "../include/Utility.h"

#ifdef WIN32
#define OS_SEP '\\'
#else
#define OS_SEP '/'
#endif

// Splits s into pieces separated by separator sign.
// Separator signs are ignored. Multiple consecutive separator signs are ignored.
std::vector<std::string> Utility::split(std::string& s, char separator)
{
    std::vector<std::string> ret;
    size_t back = 0;
    size_t front = s.find(separator, back);
    while(front != std::string::npos)
    {
        if(front - back)
        {
            ret.push_back(s.substr(back, front - back));
        }
        back = front + 1;
        s.find(separator, back); // if s ends with separator then back will be equal to s.size
    }
    if(back < s.size())
    {
        ret.push_back(s.substr(back));
    }
    return ret;
}

// Splits s into pieces separated by separator sign.
// Separator signs are ignored. Multiple consecutive separator signs are ignored.
// Merger signs allow to create split data piece with separator sign inside.
std::vector<std::string> Utility::split(std::string& s, char separator, char merger)
{
    std::vector<std::string> ret;
    size_t first = 0;
    for(size_t i = 0; i < s.size(); ++i)
    {
        if(s[i] == separator)
        {
            if(i - first)
            {
                ret.push_back(s.substr(first, i - first));
            }
            first = i + 1;
        }
        else if(s[i] == merger)
        {
            first = i + 1;
            i = s.find(merger, first);
            if(i != std::string::npos)
            {
                ret.push_back(s.substr(first, i - first));
                first = i + 1;
            }
            else
            {
                // error: odd number of *merger* signs
                return ret;
            }
        }
        else if(i == s.size() - 1)  // last data piece addition (when it is not followed by separator)
        {
            ret.push_back(s.substr(first, s.size()));
        }
    }
    return ret;
}

// Splits s into pieces separated by separator sign.
// Separator signs are ignored. Multiple consecutive separator signs are ignored.
// Merger signs allow to create split data piece with separator sign inside.
std::vector<std::string> Utility::split(std::string& s, char separator, std::vector<char> mergers)
{
    std::vector<std::string> ret;
    size_t first = 0;
    for(size_t i = 0; i < s.size(); ++i)
    {
        if(s[i] == separator)
        {
            if(i - first)
            {
                ret.push_back(s.substr(first, i - first));
            }
            first = i + 1;
        }
        else
        {
            for(char merger : mergers)
            {
                if(s[i] == merger)
                {
                    first = i + 1;
                    i = s.find(merger, first);
                    if(i != std::string::npos)
                    {
                        ret.push_back(s.substr(first, i - first));
                        first = i + 1;
                        break;
                    }
                    else
                    {
                        // odd number of *merger* signs
                        return ret;
                    }
                }
            }
        }
        if(i == s.size() - 1)  // last data piece addition (when it is not followed by separator)
        {
            ret.push_back(s.substr(first, s.size()));
        }
    }
    return ret;
}

// undefined reference during compilation when "inline" keyword is present. Why?
bool Utility::isInteger(std::string& s)
{
    for(char c : s)
    {
        if(c < '0' || c > '9')
        {
            return false;
        }
    }
    return true;
}