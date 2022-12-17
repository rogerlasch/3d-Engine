
#ifndef GPP_VALUE
#define GPP_VALUE


namespace gpp
{

enum gpp_value_types
{
gpp_type_void=0,
gpp_type_int8,
gpp_type_uint8,
gpp_type_int16,
gpp_type_uint16,
gpp_type_int32,
gpp_type_uint32,
gpp_type_int64,
gpp_type_uint64,
gpp_type_string,
gpp_type_wstring,
gpp_type_float,
gpp_type_double,
gpp_type_bool
};


class gpp_value
{
private:
union
{
bool bool_;
int64 int64_;
uint64 uint64_;
double double_;
std::string* string_;
} storage;
uint32 type;
public:
gpp_value();
gpp_value(const gpp_value& val);
gpp_value(int8 val);
gpp_value(uint8 val);
gpp_value(int16 val);
gpp_value(uint16 val);
gpp_value(int32 val);
gpp_value(uint32 val);
gpp_value(int64 val);
gpp_value(uint64 val);
gpp_value(float val);
gpp_value(double val);
gpp_value(bool val);
gpp_value(const char* s);
gpp_value(const std::string& s);
gpp_value& operator=(const gpp_value& val);
uint32 gettype()const;
void cleanup();
~gpp_value();
std::string toString()const;
bool get(int8& val)const;
bool get(uint8& val)const;
bool get(int16& val)const;
bool get(uint16& val)const;
bool get(int32& val)const;
bool get(uint32& val)const;
bool get(int64& val)const;
bool get(uint64& val)const;
bool get(float& val)const;
bool get(double& val)const;
bool get(bool& val)const;
bool get(std::string& val)const;
};

std::ostream& operator<<(std::ostream& os, gpp::gpp_value& val);

typedef std::vector<gpp::gpp_value> gpp_value_args;

template<class ...Args>
gpp_value_args gpp_make_args(const Args&... args)
{
gpp_value_args vect={args...};
return vect;
}
}
#endif
