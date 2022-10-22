

#ifndef GPP_STRING_UTILS_H
#define GPP_StRING_UTILS_H

namespace gpp
{

std::string string_to_lower_case(const std::string& s);
void string_to_lower_case(std::string& s);
std::string string_to_upper_case(const std::string& s);
void string_to_upper_case(std::string& s);
std::string string_trim(const std::string& s);
void string_trim(std::string& s);
}
#endif
