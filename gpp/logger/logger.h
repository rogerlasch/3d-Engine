


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
inline void setLevel(uint32 lv){this->level=lv;}
inline uint32 getLevel()const{return this->level;}
inline std::string getFileName()const{return filename;}

template<class... Args>
inline void write(uint32 lv, const std::string& str, const Args& ...args){
if(lv<=level){
ofn<<safe_format(str, args...)<<std::endl;
}
}
};
}
#endif
