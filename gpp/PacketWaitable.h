


#ifndef GPP_PACKETWAITABLE_H
#define GPP_PACKETWAITABLE_H

namespace gpp
{

enum PACKET_WAITABLE_FLAGS
{
PWF_WAITING=(1<<0),
PWF_READ=(1<<1),
PWF_CANCEL=(1<<2)
};

class PacketWaitable : public SafeFlags<uint32>
{
public:
packet* psend;
packet* presult;
std::atomic<int64> timestamp;
std::atomic<int64> timeout;
mutable std:: mutex mtx;
PacketWaitable();
PacketWaitable(const PacketWaitable& pw)=delete;
PacketWaitable& operator=(const PacketWaitable& pw)=delete;
virtual ~PacketWaitable();
void cleanup();
bool isWaiting()const;
bool isAnswer(packet* ps);
};
}
#endif
