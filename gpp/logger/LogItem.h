

#ifndef LOGITEM_H
#define LOGITEM_H

namespace gpp{
class LogItem{
public:
LogItem()=default;
virtual ~LogItem()=default;

virtual bool canLog()const=0;//Verifica se está pronto para gravar mensagens
virtual void init()=0;//Abre arquivo de log, inicia conexão, abre banco de dados, etc...
virtual void end()=0;//Limpa recursos
virtual void dumpMessage(uint32_t level, int64_t timestamp, const std::string& msg)=0;//Registra alguma mensagem
};
}
#endif
