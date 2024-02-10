

#ifndef INDEXTABLE_H
#define INDEXTABLE_H

#include<atomic>
#include<memory>
#include<sstream>
#include<typeinfo>
#include<vector>
#include<algorithm>

namespace gpp
{

template<class T>
class IndexTableRow
{
public:
std::atomic<uint32> vnum;
std::atomic<uint32> svcounter;
std::vector<T*> itens;
inline IndexTableRow();
IndexTableRow(const IndexTableRow& itr)=delete;
IndexTableRow& operator=(const IndexTableRow& itr)=delete;
inline virtual ~IndexTableRow();
std::string toString()const;
private:
void insertSorted(T* val);
T* get(uint32 subvnum, uint32* outindex=NULL);
inline void sort();
template<class U>
friend class IndexTable;
};

template<class T>
class IndexTable
{
public:
std::vector<std::shared_ptr<IndexTableRow<T>>> rows;
inline IndexTable();
IndexTable(const IndexTable& it)=delete;
IndexTable& operator=(const IndexTable& it)=delete;
inline virtual ~IndexTable();
inline void cleanup();
inline uint32 size(uint32 vnum=GPP_INVALID_VNUM);
inline T* create(uint32 vnum, uint32 subvnum=GPP_INVALID_VNUM);
inline T* get(const gpp_index& di);
inline T* get(uint32 vnum, uint32 subvnum);
inline std::shared_ptr<IndexTableRow<T>> getRow(uint32 vnum, uint32* outindex=NULL)const;
inline bool remove(uint32 vnum, uint32 subvnum);
inline bool removeRow(uint32 vnum);
inline void sort();
std::string toString()const;
private:
inline std::shared_ptr<IndexTableRow<T>> internal_create_row(uint32 vnum);
};
}
#endif

#ifndef INDEXTABLE_IMPLEMMENTATION
#define INDEXTABLE_IMPLEMMENTATION

namespace gpp
{
template<class T>
IndexTableRow<T>::IndexTableRow()
{
vnum.store(GPP_INVALID_VNUM);
svcounter.store(0);
itens.clear();
}

template<class T>
IndexTableRow<T>::~IndexTableRow()
{
vnum.store(GPP_INVALID_VNUM);
svcounter.store(0);
for(uint32 i=itens.size(); i>0; i--)
{
delete itens[i-1];
}
itens.clear();
}

template<class T>
std::string IndexTableRow<T>::toString()const
{
std::stringstream ss;
ss<<"{"<<itens.size()<<" colunas}";
for(auto& it : itens)
{
ss<<"{"<<it->getVnum()<<", "<<it->getSubVnum()<<"}";
}
ss<<std::endl;
return ss.str();
}

template<class T>
void IndexTableRow<T>::insertSorted(T* val)
{
if(itens.size()==0)
{
itens.push_back(val);
return;
}
bool inserted=false;
for(uint32 i=0; i<itens.size(); i++)
{
if(itens[i]->getSubVnum()>=val->getSubVnum())
{
itens.insert(itens.begin()+i, val);
inserted=true;
break;
}
}
if(!inserted)
{
itens.push_back(val);
}
}

template<class T>
T* IndexTableRow<T>::get(uint32 subvnum, uint32* outindex)
{
uint32 m1=0;
uint32 m2=itens.size()-1;
uint32 index=0;
while(m1<=m2&&m2<itens.size())
{
index=(m1+m2)/2;
if(itens[index]->getSubVnum()==subvnum)
{
if(outindex!=NULL)
{
*outindex=index;
}
return itens[index];
}
else if(subvnum<itens[index]->getSubVnum())
{
m2=(index)-1;
}
else
{
m1=(index)+1;
}
}
return NULL;
}

template<class T>
void IndexTableRow<T>::sort()
{
struct CompColumn
{
bool operator()(const T* i1, const T* i2)
{
return i1->getSubVnum()<i2->getSubVnum();
}
};
std::sort(itens.begin(), itens.end(), CompColumn());
}

//IndexTable implemmentation
template<class T>
IndexTable<T>:: IndexTable()
{
rows.clear();
}

template<class T>
IndexTable<T>::~IndexTable()
{
rows.clear();
}

template<class T>
 void IndexTable<T>::cleanup()
{
rows.clear();
}

template<class T>
 uint32 IndexTable<T>::size(uint32 vnum)
{
if(vnum==0)
{
return rows.size();
}
auto row=getRow(vnum);
return ((row==NULL) ? 0 : row->itens.size());
}

template<class T>
 T* IndexTable<T>::create(uint32 vnum, uint32 subvnum)
{
if((vnum==GPP_INVALID_VNUM)||(vnum>GPP_MAX_VNUM))
{
return NULL;
}
std::shared_ptr<IndexTableRow<T>> row=getRow(vnum);
if((row==NULL)&&((row=internal_create_row(vnum))==NULL))
{
return NULL;
}
_GASSERT(row!=NULL);
if((subvnum>0)&&(subvnum>GPP_MAX_VNUM))
{
return NULL;
}
else if((subvnum>0)&&(get(vnum, subvnum)!=NULL))
{
return NULL;
}
else if((subvnum>0)&&(row->svcounter.load()<subvnum))
{
row->svcounter.store(subvnum);
}
else if(subvnum==0)
{
row->svcounter.fetch_add(1);
subvnum=row->svcounter.load();
}
T* sf=new T();
sf->setVnum(vnum);
sf->setSubVnum(subvnum);
row->insertSorted(sf);
return sf;
}

template<class T>
 T* IndexTable<T>::get(const gpp_index& di)
{
return this->get(di.vnum, di.subvnum);
}

template<class T>
 T* IndexTable<T>::get(uint32 vnum, uint32 subvnum)
{
std::shared_ptr<IndexTableRow<T>> row=getRow(vnum);
if(row==NULL)
{
return NULL;
}
return row->get(subvnum);
}

template<class T>
std::shared_ptr<IndexTableRow<T>> IndexTable<T>::getRow(uint32 vnum, uint32* outindex)const
{
profiler_snap();
std::shared_ptr<IndexTableRow<T>> row;
if(rows.size()==0)
{
return row;
}
uint32 start=0;
uint32 end=(rows.size()-1);
uint32 index=0;
while(start<=end&&end<rows.size())
{
index=(start+end)/2;
if(rows[index]->vnum==vnum)
{
if(outindex!=NULL)
{
*outindex=index;
}
row=rows[index];
break;
}
else if(vnum<rows[index]->vnum)
{
end=(index-1);
}
else
{
start=(index+1);
}
}
return row;
}

template<class T>
 bool IndexTable<T>::remove(uint32 vnum, uint32 subvnum)
{
auto row=getRow(vnum);
if(row==NULL)
{
return false;
}
uint32 index=0;
T* val=row->get(subvnum, &index);
if(val!=NULL)
{
row->itens.erase(row->itens.begin()+index);
delete val;
return true;
}
return false;
}

template<class T>
 bool IndexTable<T>::removeRow(uint32 vnum)
{
uint32 index=0;
auto row=getRow(vnum, &index);
if(row==NULL)
{
return false;
}
rows[index]->itens.clear();
rows.erase(rows.begin()+index);
return true;
}

template<class T>
std::string IndexTable<T>::toString()const
{
std::stringstream ss;
ss<<"Imprimindo "<<typeid(rows).name()<<std::endl<<" Total de linhas: "<<rows.size()<<std::endl;
for(auto& it : rows)
{
ss<<it->toString();
}
return ss.str();
}

template<class T>
 void IndexTable<T>::sort()
{
struct CompRow
{
bool operator()(const std::shared_ptr<IndexTableRow<T>>& r1, const std::shared_ptr<IndexTableRow<T>>& r2)
{
return r1->vnum.load()<r2->vnum.load();
}
};
std::sort(rows.begin(), rows.end(), CompRow());
}

template<class T>
std::shared_ptr<IndexTableRow<T>> IndexTable<T>::internal_create_row(uint32 vnum)
{
std::shared_ptr<IndexTableRow<T>> row=make_shared<IndexTableRow<T>>();
row->vnum.store(vnum);
row->svcounter.store(0);
T* sf=new T();
sf->setVnum(vnum);
sf->setSubVnum(0);
row->insertSorted(sf);
if(rows.size()==0)
{
rows.push_back(row);
return row;
}
bool inserted=false;
for(uint32 i=0; i<rows.size(); i++)
{
if(rows[i]->vnum>=vnum)
{
rows.insert(rows.begin()+i, row);
inserted=true;
break;
}
}
if(!inserted)
{
rows.push_back(row);
}
return row;
}
}
#endif
