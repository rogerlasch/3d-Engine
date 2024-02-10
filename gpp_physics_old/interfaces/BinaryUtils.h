
/**
*Classe para lidar com corpos rígidos.
*Principalmente busca, e inserção de elementos em arrays usando o algoritmo de busca binária.
*Os métodos que levam um parãmetro do tipo shared_mutex, são para uso em multithreading.
**/
#ifndef BINARYUTILS_H
#define BINARYUTILS_H

#include<algorithm>
namespace gpp
{
struct BinaryUtils
{
template<class T>
static bool find(const std::vector<T*>& s, const gpp_index& id, uint32* index)
{
if(s.size()==0)
{
return false;
}
uint32 m1=0;
uint32 m2=s.size()-1;
uint32 x=0;
bool found=false;
while(m1<=m2&&m2<s.size())
{
x=(m1+m2)/2;
if(s[x]->index==id)
{
found=true;
break;
}
else if(s[x]->index<id)
{
m1=(x+1);
}
else
{
m2=(x-1);
}
}
if((found==true)&&(index!=NULL))
{
*index=x;
}
return found;
}

template<class T>
static  bool find(const std::vector<T*>& s, const gpp_index& id, uint32* index, std::shared_mutex& m)
{
std::shared_lock<std::shared_mutex> lck(m, std::defer_lock);
while(!lck.try_lock())
{
}
return find(s, id, index);
}

template<class T>
static  bool insert(std::vector<T*>& s, T* value)
{
struct AabbComparator
{
bool operator()(T* a1, T* a2)
{
if((a1==NULL)||(a2==NULL))
{
return false;
}
return a1->index<a2->index;
}
};
static AabbComparator abc;
if((value==NULL)||(find(s, value->index, NULL)))
{
return false;
}
if(s.size()==0)
{
s.push_back(value);
return true;
}
auto it=std::upper_bound(s.begin(), s.end(), value, abc);
s.insert(it, value);
return true;
}

template<class T>
static  bool insert(std::vector<T*>& s, T* value, std::shared_mutex& m)
{
std::unique_lock<std::shared_mutex> lck(m, std::defer_lock);
while(!lck.try_lock())
{
}
return insert(s, value);
}

template<class T>
static bool  remove(std::vector<T*>& s, const gpp_index& id, T** outptr)
{
uint32 x=0;
if(find(s, id, &x))
{
if(outptr!=NULL)
{
*outptr=s[x];
}
s.erase(s.begin()+x);
return true;
}
return false;
}

template<class T>
static bool remove(std::vector<T*>& s, const gpp_index& id, T** outptr, std::shared_mutex& m)
{
std::unique_lock<std::shared_mutex> lck(m, std::defer_lock);
while(!lck.try_lock())
{
}
return remove(s, id, outptr);
}
};
}
#endif
