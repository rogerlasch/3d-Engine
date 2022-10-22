


#ifndef CMD_PARSER_H
#define CMD_PARSER_H

namespace gpp
{

#define cmd_parse_start 0
#define cmd_parse_end 1000000

class cmd_parser
{
private:
std::string str;
std::vector<std::string> args;
public:
cmd_parser(const std::string& str="", uint32 required_args=0);
cmd_parser(const cmd_parser& ps);
cmd_parser& operator=(const cmd_parser& ps);
std::string operator[](uint32 index)const;
std::string get_args(uint32 start_index=cmd_parse_start, uint32 end=cmd_parse_end);
void cleanup();
uint32 parse_string(const std::string& str, uint32 required_args=0);
uint32 size()const;
bool exists(const std::string& arg, uint32* out_index=NULL)const;
void trim(std::string& s, const std::string& t=" \t\r\n");
};
}
#endif
