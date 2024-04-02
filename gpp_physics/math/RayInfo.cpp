
#include<sstream>
#include"../types.h"
#include"vector3d.h"
#include "rayinfo.h"

using namespace std;

namespace gpp {

    RayInfo::RayInfo() {
eDist=0.0f;
oDist=0.0f;
ePoint.zero();
oPoint.zero();
targetId=0;
}

    RayInfo::RayInfo(const RayInfo& info){
*this=info;
}

    RayInfo& RayInfo::operator=(const RayInfo& info) {
        if (this != &info) {
            eDist = info.eDist;
            oDist = info.oDist;
            ePoint = info.ePoint;
            oPoint = info.oPoint;
            targetId = info.targetId;
        }
        return *this;
    }

    string RayInfo::toString() const {
        stringstream ss;
        ss <<fixed;
ss.precision(2);
ss<<"TargetId="<<targetId<<", "<<"eDist="<<eDist<<", oDist="<<oDist<<endl;
ss<<"ePoint="<<ePoint<<", oPoint="<<oPoint<<endl;
        return ss.str();
    }

    void RayInfo::cleanup() {
        eDist = 0.0;
        oDist = 0.0;
        ePoint.zero();
        oPoint.zero();
        targetId = 0;
    }
}
