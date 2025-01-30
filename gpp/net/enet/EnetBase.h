

#ifndef ENETBASE_H
#define ENETBASE_H

namespace gpp {

class EnetBase : public NetBase {
public:
    EnetBase();
    EnetBase(const EnetBase& nt) = delete;
    EnetBase& operator=(const EnetBase& nt) = delete;
    virtual ~EnetBase();

    virtual void loadDriver();
    virtual void unloadDriver();

    virtual ClientController* createNewClient(NETEVENTCALLBACK hcall);
    virtual ServerController* createNewServer(NETEVENTCALLBACK hcall);
};
}  // namespace gpp
#endif
