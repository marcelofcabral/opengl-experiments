#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <sstream>
#include <string>

inline std::string read_file(const std::string& filename)
{
    std::ifstream file(filename);
    
    if (!file.is_open()) return "";

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    
    return stream.str();
}

inline float clamp(const float value, const float min, const float max)
{
    return value < min ? min : value > max ? max : value;
}

#endif // UTILS_H
