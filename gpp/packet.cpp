

#ifndef PICOJSON_USE_INT64
#define PICOJSON_USE_INT64
#endif
#include<picojson.h>
#include<string>
#include"types.h"
#include"debug.h"
#include"cmd_parser.h"
#include"time.h"
#include"packet.h"

using namespace std;
namespace gpp
{
atomic<uint32> packet::IdGenerator=0;
packet::packet()
{
this->reset();
}

packet::packet(const packet& p)
{
*this=p;
}

packet& packet::operator=(const packet& p)
{
this->id=p.id;
this->type=p.type;
this->errorcode=p.errorcode;
this->flags=p.flags;
this->timestamp=p.timestamp;
this->timeout=p.timeout;
this->command=p.command;
this->data=p.data;
this->errormsg=p.errormsg;
return * this;
}

packet::~packet()
{
}

void packet::reset()
{
id=0;
type=PACKET_DEFAULT;
flags=0;
errorcode=0;
timestamp=0;
timeout=0;
command="";
data="";
errormsg="";
}

bool packet::isValid()const
{
return type>=PACKET_DEFAULT&&type<PACKET_LAST;
}

bool packet::hasTimeout()const
{
return timeout>0;
}

string packet::serialize()
{
if(!this->isValid())
{
return "";
}
picojson::object obj;
timestamp=get_timestamp_ms();
obj["type"]=picojson::value(static_cast<int64>(type));
obj["timestamp"]=picojson::value(timestamp);
if(timeout>0)
{
obj["timeout"]=picojson::value(timeout);
}
if(flags>0)
{
obj["flags"]=picojson::value(static_cast<int64>(flags));
}
if(errorcode!=0)
{
obj["errorcode"]=picojson::value(static_cast<int64>(errorcode));
}
if(command.size()>0)
{
obj["command"]=picojson::value(command);
}
if(data.size()>0)
{
obj["data"]=picojson::value(data);
}
if(errormsg.size()>0)
{
obj["errormsg"]=picojson::value(errormsg);
}
return picojson::value(obj).serialize();
}

bool packet::deserialize(const string& str)
{
try {
picojson::value val;
if(picojson::parse(val, str).size()>0)
{
return false;
}
picojson::object obj=val.get<picojson::object>();
this->type=((obj.count("type")>0) ? static_cast<uint32>(obj.at("type").get<int64>()) : PACKET_DEFAULT);
this->id=((obj.count("id")>0) ? static_cast<uint32>(obj.at("id").get<int64>()) : 0);
this->flags=((obj.count("flags")>0) ? static_cast<uint32>(obj.at("flags").get<int64>()) : 0);
this->errorcode=((obj.count("errorcode")>0) ? static_cast<uint32>(obj.at("errorcode").get<int64>()) : 0);
this->timestamp=((obj.count("timestamp")>0) ? obj.at("timestamp").get<int64>() : 0);
this->timeout=((obj.count("timeout")>0) ? obj.at("timeout").get<int64>() : 0);
this->command=((obj.count("command")>0) ? obj.at("command").get<string>() : "");
this->data=((obj.count("data")>0) ? obj.at("data").get<string>() : "");
this->errormsg=((obj.count("errormsg")>0) ? obj.at("errormsg").get<string>() : "");
} catch(const exception& e) {
_LOG_EXCEPT(e.what());
this->reset();
return false;
}
return true;
}

void packet::create(uint32 type, int64 timeout, uint32 flags)
{
this->type=type;
this->flags=flags;
this->timeout=timeout;
}

void packet::createPing()
{
type=PACKET_PING;
flags=0;
timeout=0;
command="";
data="";
}

void packet::createPong()
{
type=PACKET_PONG;
flags=0;
timeout=0;
command="";
data="";
}

void packet::createCommand(const string& str, uint32 type)
{
}

void packet::createAlt(uint32 ctype, const string& name, const string& altpassword)
{
type=PACKET_ALT;
picojson::object obj;
obj["ctype"]=picojson::value(static_cast<int64>(ctype));
obj["altpassword"]=picojson::value(altpassword);
obj["name"]=picojson::value(name);
data=picojson::value(obj).serialize();
}

void packet::setCommand(const string& cmd, uint32 type)
{
switch(type)
{
case PACKET_TYPED_COMMAND:
case PACKET_GAME_COMMAND:
{
cmd_parser ps;
if(ps.parse_string(cmd, 2)<2)
{
this->command=cmd;
this->data="";
}
else
{
this->command=ps[0];
this->data=ps[1];
}
break;
}
default:
{
this->command="";
this->data=cmd;
}
}
}
}
