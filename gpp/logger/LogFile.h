

#ifndef LOGFILE_H
#define LOGFILE_H

namespace gpp{
class LogFile : public LogItem{
private:
std::string filename;
std::ofstream ofn;
bool print;
public:
LogFile(const std::string& filename="", bool print=false);
virtual ~LogFile()=default;

virtual inline bool canLog()const{return ofn.is_open();}
virtual void init();//Abre arquivo de log, inicia conexão, abre banco de dados, etc...
virtual void end();//Limpa recursos
virtual void dumpMessage(uint32 level, int64_t timestamp, const std::string& msg);//Registra alguma mensagem
};
}
#endif
