

#ifndef SAFEFLAGS_H
#define SAFEFLAGS_H

namespace gpp
{

template<class T>
class SafeFlags
{
private:
std::atomic<T> flags;
public:
inline SafeFlags();
inline SafeFlags(T f);
SafeFlags(const SafeFlags& sf)=delete;
SafeFlags& operator=(const SafeFlags& sf)=delete;
inline virtual ~SafeFlags();
inline void replace_flags(T f);
inline T get_flags()const;
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
flags.store(0);
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
this->flags.store(f);
}

template<class T>
T SafeFlags<T>::get_flags()const
{
return flags.load();
}

template<class T>
bool SafeFlags<T>::flag_contains(T f)const
{
return ((this->flags&f)==f);
}

template<class T>
bool SafeFlags<T>::setflag(T f)
{
if(this->flag_contains(f))
{
return false;
}
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
flags^=f;
return true;
}
}
#endif
