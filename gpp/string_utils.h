

#ifndef GPP_STRING_UTILS_H
#define GPP_StRING_UTILS_H

#include<vector>

namespace gpp
{

std::string string_to_lower_case(const std::string& s);
std::wstring wstring_to_lower_case(const std::wstring& s);
void string_to_lower_case(std::string& s);
void wstring_to_lower_case(std::wstring& s);
std::string string_to_upper_case(const std::string& s);
std::wstring wstring_to_upper_case(const std::wstring& s);
void string_to_upper_case(std::string& s);
void wstring_to_upper_case(std::wstring& s);
std::string string_trim(const std::string& s);
std::wstring wstring_trim(const std::wstring& s);
void string_trim(std::string& s);
void wstring_trim(std::wstring& s);
std::string string_replace(const std::string& str, const std::string& old, const std::string& new_str);
std::string string_replace(const std::string& str, const std::vector<std::pair<std::string, std::string>>& args);
std::wstring wstring_replace(const std::wstring& str, const std::wstring& old, const std::wstring& new_str);
std::wstring wstring_replace(const std::wstring& str, const std::vector<std::pair<std::wstring, std::wstring>>& args);
std::string string_utf8_encode(const std::wstring& str);
std::wstring string_utf8_decode(const std::string& str);
std::string wstring_to_string(const std::wstring& str);
std::wstring string_to_wstring(const std::string& str);
}
#endif
