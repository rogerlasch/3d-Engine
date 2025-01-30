

#ifndef GPP_SAFE_FORMAT_H
#define GPP_SAFE_FORMAT_H

#include<format>
#include<string>

namespace gpp{

template<class ...Args>
inline std::string safe_format(const std::string& str, const Args& ...args)
{
std::string final="";
try {
final=std::vformat(str, std::make_format_args(args...));
} catch(const std::exception& e) {
e.what();
return "";
}
return final;
}

template<class ...Args>
inline std::wstring safe_wformat(const std::wstring& str, const Args& ...args)
{
std::wstring final=L"";
try {
final=std::vformat(str, std::make_wformat_args(args...));
} catch(const std::exception& e) {
e.what();
return L"";
}
return final;
}
}
#endif
