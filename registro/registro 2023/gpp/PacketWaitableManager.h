

#ifndef PACKETWAITABLEMANAGER_H
#define PACKETWAITABLEMANAGER_H

namespace gpp{

enum GPP_PACKET_WAITABLE_STATES{
PW_DEFAULT=0,
PW_WAITING,
PW_ERROR,
PW_TIMEOUT,
PW_READY,
};
typedef std::function<void(uint32, Packet*, Packet*)> PACKETCALLBACK;

struct PW_ITEM{
uint32 hstate=0;
Packet* hsend=NULL;
Packet* hreseive=NULL;
};
typedef std::shared_ptr<PW_ITEM> shared_pw_item;

class PacketWaitableManager{
private:
std::unordered_map<std::string, shared_pw_item> itens;
PACKETCALLBACK hcall;
public:
PacketWaitableManager(PACKETCALLBACK  hcall=NULL);
PacketWaitableManager(const PacketWaitableManager& s)=delete;
PacketWaitableManager& operator=(const PacketWaitableManager& s)=delete;
virtual ~PacketWaitableManager();
std::string toString()const;
void cleanup();
void add(Packet* pack);
void remove(const std::string& cmd);
bool exists(const std::string& cmd)const;
bool getAnswer(const std::string& cmd, Packet** pack);

};
}
#endif
