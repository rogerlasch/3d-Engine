


#ifndef RALERT_H
#define RALERT_H

namespace rgames
{

#define RA_REPEATE 350

class ralert : public RdialogBase
{
private:
ruint32 handle;
std::string msg;
std::string soundfile;
public:
ralert(const std::string& msg="", const std::string& soundfile="");
ralert(const ralert& ra)=delete;
ralert& operator=(const ralert& ra)=delete;
virtual ~ralert();
virtual void enable(bool en);
virtual void showModal();
virtual void endModal();
virtual ruint32 handleEvent(rint32 hstate, rint32 hkey);
virtual rint32 key_to_event(rint32 hstate, rint32 hkey);
};
}
#endif
