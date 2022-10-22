
#include<string.h>
#include<string>
#include<iostream>
#include<vector>
#include"types.h"
#include"gpp_value.h"

using namespace std;

namespace gpp
{

gpp_value::gpp_value()
{
type=gpp_type_void;
memset(&storage, 0, sizeof(storage));
}

gpp_value::gpp_value(const gpp_value& val)
{
*this=val;
}

gpp_value::gpp_value(int8 val)
{
type=gpp_type_int8;
storage.int64_=val;
}

gpp_value::gpp_value(uint8 val)
{
type=gpp_type_uint8;
storage.uint64_=val;
}

gpp_value::gpp_value(int16 val)
{
type=gpp_type_int16;
storage.int64_=val;
}

gpp_value::gpp_value(uint16 val)
{
type=gpp_type_uint16;
storage.uint64_=val;
}

gpp_value::gpp_value(int32 val)
{
type=gpp_type_int32;
storage.int64_=val;
}

gpp_value::gpp_value(uint32 val)
{
type=gpp_type_uint32;
storage.uint64_=val;
}

gpp_value::gpp_value(int64 val)
{
type=gpp_type_int64;
storage.int64_=val;
}

gpp_value::gpp_value(uint64 val)
{
type=gpp_type_uint64;
storage.uint64_=val;
}

gpp_value::gpp_value(float val)
{
type=gpp_type_float;
storage.double_=val;
}

gpp_value::gpp_value(double val)
{
type=gpp_type_double;
storage.double_=val;
}

gpp_value::gpp_value(bool val)
{
type=gpp_type_bool;
storage.bool_=val;
}

gpp_value::gpp_value(const char* s)
{
type=gpp_type_string;
storage.string_=new string(((s==NULL) ? "" : s));
}

gpp_value::gpp_value(const string& s)
{
type=gpp_type_string;
storage.string_=new string(s);
}

gpp_value& gpp_value::operator=(const gpp_value& val)
{
this->cleanup();
this->type=val.type;
switch(type)
{
case gpp_type_string:
{
storage.string_=new string(*val.storage.string_);
break;
}
default:
{
memcpy(&storage, &val.storage, sizeof(val.storage));
break;
}
}
return *this;
}

gpp_value::~gpp_value()
{
switch(type)
{
case gpp_type_string:
{
delete storage.string_;
memset(&storage, 0, sizeof(storage));
break;
}
default:
{
memset(&storage, 0, sizeof(storage));
}
}
}

uint32 gpp_value::gettype()const
{
return type;
}

void gpp_value::cleanup()
{
switch(type)
{
case gpp_type_string:
{
delete storage.string_;
memset(&storage, 0, sizeof(storage));
break;
}
default:
{
memset(&storage, 0, sizeof(storage));
}
}
type=gpp_type_void;
}

string gpp_value::to_string()const
{
switch(type)
{
case gpp_type_string:
{
return *storage.string_;
}
case gpp_type_bool:
{
return ((storage.bool_==true) ? "true" : "false");
}
case gpp_type_int8:
case gpp_type_int16:
case gpp_type_int32:
case gpp_type_int64:
{
return std::to_string(storage.int64_);
}
case gpp_type_uint8:
case gpp_type_uint16:
case gpp_type_uint32:
case gpp_type_uint64:
{
return std::to_string(storage.uint64_);
}
case gpp_type_float:
case gpp_type_double:
{
return std::to_string(storage.double_);
}
default:
{
return "void";
}
}
}

bool gpp_value::get( int8& value)const
{
if(type!=gpp_type_int8)
{
return false;
}
value=static_cast<int8>(storage.int64_);
return true;
}

bool gpp_value::get( uint8& value)const
{
if(type!=gpp_type_uint8)
{
return false;
}
value=static_cast<uint8>(storage.uint64_);
return true;
}

bool gpp_value::get( int16& value)const
{
if(type!=gpp_type_int16)
{
return false;
}
value=static_cast<int16>(storage.int64_);
return true;
}

bool gpp_value::get( uint16& value)const
{
if(type!=gpp_type_uint16)
{
return false;
}
value=static_cast<uint16>(storage.uint64_);
return true;
}

bool gpp_value::get( int32& value)const
{
if(type!=gpp_type_int32)
{
return false;
}
value=static_cast<int32>(storage.int64_);
return true;
}

bool gpp_value::get( uint32& value)const
{
if(type!=gpp_type_uint32)
{
return false;
}
value=static_cast<uint32>(storage.uint64_);
return true;
}

bool gpp_value::get( int64& value)const
{
if(type!=gpp_type_int64)
{
return false;
}
value=storage.int64_;
return true;
}

bool gpp_value::get( uint64& value)const
{
if(type!=gpp_type_uint64)
{
return false;
}
value=storage.uint64_;
return true;
}

bool gpp_value::get( float& value)const
{
if(type!=gpp_type_float)
{
return false;
}
value=static_cast<float>(storage.double_);
return true;
}

bool gpp_value::get( double& value)const
{
if(type!=gpp_type_double)
{
return false;
}
value=storage.double_;
return true;
}

bool gpp_value::get( bool& value)const
{
if(type!=gpp_type_bool)
{
return false;
}
value=storage.bool_;
return true;
}

bool gpp_value::get( string& value)const
{
if(type!=gpp_type_string)
{
return false;
}
value=*storage.string_;
return true;
}

ostream& operator<<(ostream& os, gpp_value& val)
{
os<<val.to_string();
return os;
}
}
