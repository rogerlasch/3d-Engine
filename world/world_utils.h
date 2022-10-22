


#ifndef WORLD_UTILS_H
#define WORLD_UTILS_H
namespace gpp
{

template<class T>
inline void gpp_try_lock(T& lck)
{
while(!lck.try_lock())
{
wait_mcs(random_uint32(5, 999));
}
}

template<class T>
struct BodyComparator
{
bool operator()(const T* b, const gpp_index& id)const
{
profiler_snap();
return b->index<id;
}
bool isMinor(const T* b, const gpp_index& id)const
{
profiler_snap();
return b->index<id;
}
bool equals(const T* b, const gpp_index& id)const
{
profiler_snap();
return b->index==id;
}
bool isMajor(const T* b, const gpp_index& id)const
{
profiler_snap();
return !b->index<id;
}
bool operator()(const T* b1, const T* b2)const
{
profiler_snap();
return b1->index<b2->index;
}
bool isMinor(const T* b1, const T* b2)const
{
profiler_snap();
return b1->index<b2->index;
}
bool equals(const T* b1, const T* b2)const
{
profiler_snap();
return b1->index==b2->index;
}
bool isMajor(const T* b1, const T* b2)const
{
profiler_snap();
return !b1->index<b2->index;
}
};

struct BodyDeletor
{
template<class T>
void operator()(T* value)
{
profiler_snap();
if(value)
{
delete value;
}
}
};

template<class T>
struct RangeFinder
{
bool operator()(const std::vector<T*>& arr, uint32 start_index, uint32* start, uint32* end)
{
profiler_snap();
if((start==NULL)||(end==NULL)||(arr.size()==0)||(start_index>arr.size()-1))
{
return false;
}
*start=findLeft(arr, start_index);
*end=findRight(arr, start_index);
return true;
}
bool operator()(const std::vector<T*>& arr, uint32 start_index, uint32* start, uint32* end, std::shared_mutex& mtx)
{
std::shared_lock lck(mtx, std::defer_lock);
gpp_try_lock(lck);
return *this(arr, start_index, start, end);
}
uint32 findLeft(const std::vector<T*>& arr, uint32 start_index)
{
profiler_snap();
uint32 i=start_index;
BodyComparator<T> cp;
for(i=start_index; i>0; i--)
{
if(!cp.equals(arr[start_index], arr[i-1]))
{
break;
}
}
return i;
}
uint32 findRight(const std::vector<T*>& arr, uint32 start_index)
{
profiler_snap();
uint32 i=start_index;
BodyComparator<T> cp;
for(int i=start_index; i<arr.size()-1; i++)
{
if(!cp.equals(arr[start_index], arr[i+1]))
{
break;
}
}
return i;
}
};

template<class T>
struct BinarySearcher
{
bool findRange(const std::vector<T*>& arr, const gpp_index& val, uint32* start, uint32* end)
{
profiler_snap();
if((arr.size()==0)||(val==NULL))
{
return false;
}
uint32 m1=0;
uint32 m2=arr.size()-1;
uint32 x=0;
BodyComparator<T> cp;
RangeFinder<T> rf;
while(m1<=m2&&m2<arr.size())
{
x=(m1+m2)/2;
if(cp.equals(arr[x], val))
{
rf(arr, x, start, end);
return true;
}
else if(cp(arr[x], val))
{
m1=(x+1);
}
else
{
m2=(x-1);
}
}
return false;
}
bool findRange(const std::vector<T*>& arr, const gpp_index& val, uint32* start, uint32* end, std::shared_mutex& mtx)
{
std::shared_lock lck(mtx, std::defer_lock);
gpp_try_lock(lck);
return this->findRange(arr, val, start, end);
}
bool findRange(const std::vector<T*>& arr, const T* val, uint32* start, uint32* end)
{
profiler_snap();
if((arr.size()==0)||(val==NULL))
{
return false;
}
uint32 m1=0;
uint32 m2=arr.size()-1;
uint32 x=0;
BodyComparator<T> cp;
RangeFinder<T> rf;
while(m1<=m2&&m2<arr.size())
{
x=(m1+m2)/2;
if(cp.equals(arr[x], val))
{
rf(arr, x, start, end);
return true;
}
else if(cp(arr[x], val))
{
m1=(x+1);
}
else
{
m2=(x-1);
}
}
return false;
}
bool findRange(const std::vector<T*>& arr, const T* val, uint32* start, uint32* end, std::shared_mutex& mtx)
{
std::shared_lock lck(mtx, std::defer_lock);
gpp_try_lock(lck);
return this->findRange(arr, val, start, end);
}
bool findUnique(const std::vector<T*>& arr, const gpp_index& val, uint32* outindex)
{
profiler_snap();
if((arr.size()==0)||(val==NULL))
{
return false;
}
uint32 m1=0;
uint32 m2=arr.size()-1;
uint32 x=0;
BodyComparator<T> cp;
RangeFinder<T> rf;
while(m1<=m2&&m2<arr.size())
{
x=(m1+m2)/2;
//_FLOG("Testando {},{} com {},{}", arr[x]->index.vnum, arr[x]->index.subvnum, val.vnum, val.subvnum);
if(cp.equals(arr[x], val))
{
if(outindex!=NULL)
{
uint32 start=0, end=0;
rf(arr, x, &start, &end);
*outindex=start;
}
return true;
}
else if(cp(arr[x], val))
{
m1=(x+1);
}
else
{
m2=(x-1);
}
}
return false;
}
bool findUnique(const std::vector<T*>& arr, const gpp_index& val, uint32* outindex, std::shared_mutex& mtx)
{
std::shared_lock lck(mtx, std::defer_lock);
gpp_try_lock(lck);
return this->findUnique(arr, val, outindex);
}
bool findUnique(const std::vector<T*>& arr, const T* val, uint32* outindex)
{
profiler_snap();
if((arr.size()==0)||(val==NULL))
{
return false;
}
uint32 m1=0;
uint32 m2=arr.size()-1;
uint32 x=0;
BodyComparator<T> cp;
RangeFinder<T> rf;
while(m1<=m2&&m2<arr.size())
{
x=(m1+m2)/2;
if(cp.equals(arr[x], val))
{
if(outindex!=NULL)
{
uint32 start=0, end=0;
rf(arr, x, &start, &end);
*outindex=start;
}
return true;
}
else if(cp(arr[x], val))
{
m1=(x+1);
}
else
{
m2=(x-1);
}
}
return false;
}
bool findUnique(const std::vector<T*>& arr, const T* val, uint32* outindex, std::shared_mutex& mtx)
{
std::shared_lock lck(mtx, std::defer_lock);
gpp_try_lock(lck);
return this->findUnique(arr, val, outindex);
}
};

template<class T>
struct InserterSorted
{
void operator()(std::vector<T*>& arr, T* val)
{
profiler_snap();
BodyComparator<T> cp;
if(arr.size()==0)
{
arr.push_back(val);
return;
}
auto it=std::upper_bound(arr.begin(), arr.end(), val, cp);
arr.insert(it, val);
}
void operator()(std::vector<T*>& arr, T* val, std::shared_mutex& mtx)
{
std::unique_lock<std::shared_mutex> lck(mtx, std::defer_lock);
gpp_try_lock(lck);
*this(arr, val);
}
};
}
#endif
