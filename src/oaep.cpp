#include "oaep.h"

std::string OAEP::addPadding(std::string input)
{
    std::string padding((n / 8) - input.size(), '0');
    return input.append(padding);
}