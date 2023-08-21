

#ifndef RSTRING_UTILS_H
#define RStRING_UTILS_H

#include<vector>

namespace rgames
{

std::string rstring_to_lower_case(const std::string& s);
std::wstring rwstring_to_lower_case(const std::wstring& s);
void rstring_to_lower_case(std::string& s);
void rwstring_to_lower_case(std::wstring& s);
std::string rstring_to_upper_case(const std::string& s);
std::wstring rwstring_to_upper_case(const std::wstring& s);
void rstring_to_upper_case(std::string& s);
void rwstring_to_upper_case(std::wstring& s);
std::string rstring_trim(const std::string& s);
std::wstring rwstring_trim(const std::wstring& s);
void rstring_trim(std::string& s);
void rwstring_trim(std::wstring& s);
std::string rstring_replace(const std::string& str, const std::string& old, const std::string& new_str);
std::string rstring_replace(const std::string& str, const std::vector<std::pair<std::string, std::string>>& args);
std::wstring rwstring_replace(const std::wstring& str, const std::wstring& old, const std::wstring& new_str);
std::wstring rwstring_replace(const std::wstring& str, const std::vector<std::pair<std::wstring, std::wstring>>& args);
std::string rstring_utf8_encode(const std::wstring& str);
std::wstring rstring_utf8_decode(const std::string& str);
std::string rwstring_to_string(const std::wstring& str);
std::wstring rstring_to_wstring(const std::string& str);
}
#endif
