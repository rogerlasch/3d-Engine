


#ifndef RDIALOGBASE_H
#define RDIALOGBASE_H

namespace rgames
{

enum RDIALOG_CONSTS
{
RD_CONTINUE=100,
RD_CANCEL,
RD_SELECT,
RD_FINISH
};

class RdialogBase
{
protected:
bool enabled=true;
RdialogBase* next;
public:
inline RdialogBase(){
enabled=true;
this->next=NULL;
}
RdialogBase(const RdialogBase& rd)=delete;
RdialogBase& operator=(const RdialogBase& rd)=delete;
inline virtual ~RdialogBase()
{
this->next=NULL;
}
inline bool isEnabled()const{return this->enabled;}
inline RdialogBase* getNext()const {return this->next;}
inline void setNext(RdialogBase* n){this->next=n;}
inline virtual void enable(bool en){this->enabled=en;}
inline virtual void showModal(){}
inline virtual void endModal(){}
inline virtual ruint32 handleEvent(rint32 hstate, rint32 hkey){return RD_FINISH;}
inline virtual rint32 key_to_event(rint32 hstate, rint32 hkey){return 0;}
};
}
#endif
