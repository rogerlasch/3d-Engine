

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
std::string string_utf8_encode(const std::wstring& str);
std::wstring string_utf8_decode(const std::string& str);
std::string wstring_to_string(const std::wstring& str);
std::wstring string_to_wstring(const std::string& str);
}
#endif
