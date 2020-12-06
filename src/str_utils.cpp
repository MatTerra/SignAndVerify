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

std::string toBinary(std::string input)
{
    std::string output = "";
    for (char &_char : input)
    {
        output += std::bitset<8>(_char).to_string();
    }
    return output;
}

void fromBinary(char *output, std::string input)
{
    std::stringstream sstream(input);
    char c = '\0';
    int i = 0;
    while (sstream.good())
    {
        std::bitset<8> bits;
        sstream >> bits;
        c = char(bits.to_ulong());
        output[i++] = c;
    }
    output[i] = '\0';
}