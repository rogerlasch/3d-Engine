
#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>
#include<atomic>
#include<mutex>
#include<shared_mutex>
#include<string>
#include"types.h"
#include"debug.h"
#include"basic_peer.h"

using namespace std;

namespace gpp
{
basic_peer::basic_peer()
{
peer_id=0;
hserver=NULL;
connection_time=0;
hstate=PEER_DEFAULT;
}

basic_peer::~basic_peer()
{
}

bool basic_peer::isConnected()const
{
return hstate.load()==PEER_CONNECTED;
}

bool basic_peer::isDisconnected()const
{
return hstate.load()==PEER_DISCONNECTED;
}

bool basic_peer::isAlt()const
{
return hstate.load()==PEER_ALT;
}

uint32  basic_peer::getPeerId()const
{
shared_lock<shared_mutex> lck(mtx_con);
return peer_id;
}

void  basic_peer::setPeerId(uint32 peer_id)
{
unique_lock<shared_mutex> lck(mtx_con);
this->peer_id=peer_id;
}

uint32  basic_peer::getHstate()const
{
return hstate.load();
}

void  basic_peer::setHstate(uint32 hstate)
{
this->hstate.store(hstate);
}

int64  basic_peer::getConnectionTime()const
{
shared_lock<shared_mutex> lck(mtx_con);
return this->connection_time;
}

void  basic_peer::setConnectionTime(int64 connection_time)
{
unique_lock<shared_mutex> lck(mtx_con);
this->connection_time=connection_time;
}

ISteamNetworkingSockets*  basic_peer::getHserver()const
{
shared_lock<shared_mutex> lck(mtx_con);
return this->hserver;
}

void  basic_peer::setHserver(ISteamNetworkingSockets* hserver)
{
unique_lock<shared_mutex> lck(mtx_con);
this->hserver=hserver;
}

bool basic_peer::sendUnreliable(const std::string& data)
{
unique_lock<shared_mutex> lck(mtx_con);
if((hserver==NULL)||(peer_id==0))
{
return false;
}
uint32 x=hstate.load();
if((x!=PEER_CONNECTED)&&(x!=PEER_DISCONNECTING))
{
return false;
}
uint32 res=hserver->SendMessageToConnection(peer_id, data.c_str(), data.size(), k_nSteamNetworkingSend_Unreliable, NULL);
return res==k_EResultOK;
}

bool basic_peer::sendReliable(const std::string& data)
{
unique_lock<shared_mutex> lck(mtx_con);
if((hserver==NULL)||(peer_id==0))
{
_FLOG("Peer ou hserver");
return false;
}
uint32 x=hstate.load();
if((x!=PEER_CONNECTED)&&(x!=PEER_DISCONNECTING))
{
return false;
}
uint32 res=hserver->SendMessageToConnection(peer_id, data.c_str(), data.size(), k_nSteamNetworkingSend_Reliable, NULL);
return res==k_EResultOK;
}

bool basic_peer::disconnect()
{
unique_lock<shared_mutex> lck(mtx_con);
if((hserver==NULL)||(peer_id==0))
{
return false;
}
if(hserver->CloseConnection(peer_id, 0, NULL, true))
{
hstate.store(PEER_DISCONNECTING);
return true;
}
return false;
}

bool basic_peer::disconnectNow()
{
unique_lock<shared_mutex> lck(mtx_con);
if((hserver==NULL)||(peer_id==0))
{
return false;
}
if(hserver->CloseConnection(peer_id, 0, NULL, false))
{
hstate.store(PEER_DISCONNECTED);
return true;
}
return false;
}

void basic_peer::updateCon()
{
switch(this->getHstate())
{
case PEER_ALT:
case PEER_CONNECTED:
{
unique_lock<shared_mutex> lck(mtx_con);
if((hserver==NULL)||(peer_id==0))
{
return;
}
SteamNetworkingMessage_t * msgs=NULL;
uint32 max_messages=10;
int32 size=0;
hserver->FlushMessagesOnConnection(peer_id);
if((size=hserver->ReceiveMessagesOnConnection(peer_id, &msgs, max_messages))<=0)
{
return;
}
for(int32 i=0; i<size; i++)
{
//Gera evento e despacha a mensagem como evento...
string data="";
data.assign((char*)msgs[i].m_pData, msgs[i].m_cbSize);
msgs[i].Release();
_FLOG("MSG: {}", data);
}
break;
}
}
}
}
