
#ifndef NETOBJECT_H
#define NETOBJECT_H

#include<atomic>
#include<functional>
#include<thread>
#include<mutex>
#include"../debug_system.h"
#include"../MemoryPool.h"
#include"NetCodes.h"
#include"NetEvent.h"
#include"Packet.h"

namespace gpp {
class NetObject {
protected:
    NETEVENTCALLBACK hcall;
    std::atomic<uint32> hstate;
MemoryPool<NetEvent> eventPool;
MemoryPool<Packet> packetPool;
    mutable std::mutex mtx;

public:
inline    NetObject() : hstate(GNT_DEFAULT) {
eventPool.enablePool(true);
packetPool.enablePool(true);
}

inline     virtual ~NetObject(){
eventPool.cleanup();
packetPool.cleanup();
}

inline Packet* createPacket(uint32 type, const std::string& data, int64 timestamp){
Packet* hpack=packetPool.getNewObject();
hpack->type=type;
hpack->data=data;
hpack->timestamp=timestamp;
return hpack;
}

inline void releasePacket(Packet* hpack){
packetPool.releaseObject(hpack);
}

inline     virtual void init() {}
inline     virtual void end() {}
inline     virtual void shutdown(){}
inline     virtual void update() {}

inline     void setEventCallback(NETEVENTCALLBACK hcall) {
        this->hcall = hcall;
    }

inline     NETEVENTCALLBACK getEventCallback() const {
        return hcall;
    }

inline     void setState(uint32 st) {
        this->hstate.store(st);
    }

inline     uint32 getState() const {
        return hstate.load();
    }

inline     void sendEvent(uint32 type, uint32 peer_id, const std::string& data) {
        if (hcall) {
NetEvent* ev=eventPool.getNewObject();
ev->type=type;
ev->id=peer_id;
ev->data=data;
            hcall(ev);
eventPool.releaseObject(ev);
        }
    }

    template <class... Args>
inline     void trace(const std::string& data, const Args&... args) {
        if (hcall) {
            std::string final = safe_format(data, args...);
NetEvent* ev=eventPool.getNewObject();
ev->type=GNET_LOG;
ev->id=0;
ev->data=final;
            hcall(ev);
eventPool.releaseObject(ev);
        }
    }
};
}  // namespace gpp
#endif
