

#ifndef GPP_COMMAND_H
#define GPP_COMMAND_H

namespace gpp
{

enum GPP_COMMAND_FLAGS
{
CMD_PROTOCOL=(1<<0),
CMD_NPC=(1<<1),
CMD_CHAR=(1<<2),
CMD_ADM=(1<<3),
CMD_DISABLED=(1<<4)
};

typedef std::function<void(void*, const std::string&)>  CMD_PROTO_FUNC;

class command : public SafeFlags<uint32>
{
public:
std::string name;
CMD_PROTO_FUNC hfunc;
command();
command(const command& c)=delete;
command& operator=(const command& c)=delete;
bool isValid()const;
};
}
#endif
