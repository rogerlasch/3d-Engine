


#ifndef LOGGER_H
#define LOGGER_H

namespace gpp
{

class logger
{
public:
int32 level;
uint32 flags;
std::string filename;
std::vector<std::string> stacktrace;
std::ofstream ofn;
logger(const std::string& filename, int32 level);
logger(const logger& lg)=delete;
logger& operator=(const logger& lg)=delete;
virtual ~logger();
};
}
#endif
