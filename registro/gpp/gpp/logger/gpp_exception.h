

#ifndef GPP_EXCEPTION_H
#define GPP_EXCEPTION_H

#include<exception>

namespace gpp
{

class gpp_exception : public std::exception
{
private:
std::string msg;
public:
gpp_exception(const std::string& msg);
gpp_exception(const std::string& msg, const std::string& function_name, const std::string& filename, int line);
gpp_exception(const gpp_exception& gx);
gpp_exception& operator=(const gpp_exception& gx);
virtual ~gpp_exception();
virtual const char* what() const noexcept override;
};
#define _GEXCEPT(msg) gpp_exception(msg, __FUNCTION__, __FILE__, __LINE__)
}
#endif
