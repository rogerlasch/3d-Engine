


#ifndef GPP_PACKET_H
#define GPP_PACKET_H

namespace gpp
{

enum PACKET_TYPES
{
PACKET_DEFAULT=0,
PACKET_PING,
PACKET_PONG,
PACKET_ALT,
PACKET_TYPED_COMMAND,
PACKET_GAME_COMMAND,
PACKET_SOUND,
PACKET_WAITABLE,
PACKET_ANSWER_WAITABLE,
PACKET_LAST
};

//Usado com pacotes waitable...
enum PACKET_WAITABLE_STATUS
{
PWAITABLE_DEFAULT=0,//Não serializar isto...
PWAITABLE_OK,//sucesso
PWAITABLE_ERROR,//A resposta foi negativa. Verifique a mensagem de erro.
PWAITABLE_TIMEOUT,//O Tempo de espera foi esgotado.
PWAITABLE_UNKNOWN //Erro desconhecido!
};

class packet
{
public:
uint32 type;
uint32 subtype;
uint32 errorcode;
uint32 flags;
int64 timestamp;
int64 timeout;
std::string command;
std::string data;
std::string errormsg;
packet();
packet(const packet& p);
packet& operator=(const packet& p);
virtual ~packet();
void reset();
bool isValid()const;
bool hasTimeout()const;
bool isWaitable()const;
std::string serialize();
bool deserialize(const std::string& str);
void create(uint32 type, int64 timeout=0, uint32 flags=0);
void createPing();
void createPong();
void createCommand(const std::string& str, uint32 type=PACKET_GAME_COMMAND);
void createAlt(uint32 ctype, const std::string& name, const std::string& altpassword);
void setCommand(const std::string& cmd, uint32 type=PACKET_GAME_COMMAND);
};
}
#endif
