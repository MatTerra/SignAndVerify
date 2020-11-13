#include "str_utils.h"

std::string replaceSubString(std::string input, std::string substring, std::string replacement)
{
    for (size_t index = 0; true; index += substring.length())
    {
        /* Locate the substring to replace. */
        index = input.find(substring, index);
        if (index == std::string::npos)
            break;

        /* Make the replacement. */
        input.erase(index, substring.length());
        input.insert(index, replacement);
    }
    return input;
}