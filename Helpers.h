#ifndef HELPERS_H
#define HELPERS_H

#include <sstream>
#include <string>

// Generic number → string converter (C++98 compatible)
template <typename T>
std::string numToStr(T value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

#endif // HELPERS_H
