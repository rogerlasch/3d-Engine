

#ifndef SAFEFLAGS_H
#define SAFEFLAGS_H

namespace gpp
{

template<class T>
class SafeFlags
{
private:
T flags;
mutable std::shared_mutex mtx_flags;
public:
inline SafeFlags();
inline SafeFlags(T f);
SafeFlags(const SafeFlags& sf)=delete;
SafeFlags& operator=(const SafeFlags& sf)=delete;
inline virtual ~SafeFlags();
inline void replace_flags(T f);
inline bool flag_contains(T f)const;
inline bool setflag(T f);
inline bool removeflag(T f);
};
}
#endif

#ifndef SAFEFLAGS_IMPLEMENTATION
#define SAFEFLAGS_IMPLEMENTATION

namespace gpp
{

template<class T>
SafeFlags<T>::SafeFlags()
{
std::memset(&flags, 0, sizeof(flags));
}

template<class T>
SafeFlags<T>::SafeFlags(T f)
{
replace_flags(f);
}

template<class T>
SafeFlags<T>::~SafeFlags()
{
}

template<class T>
void SafeFlags<T>::replace_flags(T f)
{
std::unique_lock<std::shared_mutex> lck(this->mtx_flags);
this->flags=f;
}

template<class T>
bool SafeFlags<T>::flag_contains(T f)const
{
std::shared_lock<std::shared_mutex> lck(this->mtx_flags);
return ((this->flags&f)==f);
}

template<class T>
bool SafeFlags<T>::setflag(T f)
{
if(this->flag_contains(f))
{
return false;
}
std::unique_lock<std::shared_mutex> lck(this->mtx_flags);
flags|=f;
return true;
}

template<class T>
bool SafeFlags<T>::removeflag(T f)
{
if(!this->flag_contains(f))
{
return false;
}
std::unique_lock<std::shared_mutex> lck(this->mtx_flags);
flags^=f;
return true;
}
}
#endif
