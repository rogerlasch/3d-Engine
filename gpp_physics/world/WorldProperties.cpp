
#include<thread>
#include<gpp/debug_system.h>
#include"../math/math.h"
#include <sstream>
#include"WorldProperties.h"

using namespace std;

namespace gpp {

    WorldProperties::WorldProperties()     {
this->cleanup();
}

    WorldProperties::WorldProperties(const WorldProperties& wp) {
*this=wp;
}

    WorldProperties& WorldProperties::operator=(const WorldProperties& wp) {
        if (this != &wp) {
            linearDamping = wp.linearDamping;
            angularDamping = wp.angularDamping;
fps=wp.fps;
frameTime=wp.frameTime;
currentTime=wp.currentTime;
timeStep=wp.timeStep;
            radius = wp.radius;
currentFrame=wp.currentFrame;
            gravity = wp.gravity;
            center = wp.center;
        }
        return *this;
    }

    string WorldProperties::toString() const {
        stringstream ss;
ss<<fixed;
ss.precision(2);

ss<<"Tempo total da simulação: "<<currentTime<<endl;
ss<<"Frame atual: "<<currentFrame<<endl;
ss<<"FPS: "<<fps<<endl;
ss<<"FrameTime: "<<frameTime<<endl;
ss<<"timeStep: "<<timeStep<<endl;
        ss << "Linear Damping: " << linearDamping << endl;
           ss<< "Angular Damping: " << angularDamping << endl;
           ss<< "Radius: " << radius << endl;
           ss<< "Gravity: "<<gravity<<endl;
           ss<< "Center: "<<center<<endl;
        return ss.str();
    }

    void WorldProperties::cleanup() {
currentTime=0.0f;
fps=0;
frameTime=0;
timeStep=0.0f;
        linearDamping = 0.0f;
        angularDamping = 0.0f;
        radius = 0.0f;
currentFrame=0;
        gravity.zero();
        center.zero();
    }

    void WorldProperties::toDefault() {
currentTime=0.0f;
fps=35;
frameTime=(int64)1000/(int64)fps;
timeStep=1.0f/(decimal)fps;
        linearDamping =0.3f;
        angularDamping = 0.2f;
        radius =(1024.0f*1024.0f)*1024.0f;
currentFrame=0;
        gravity ={0.0f, 0.0f, -9.81f};
        center.zero();
lastFrameTime=mClock::now();
    }

void WorldProperties::advance(decimal dt){
timeStep=dt;
currentTime+=timeStep;
currentFrame+=1;
}

int64 WorldProperties::sleepTime() {
mClock::time_point tend=mClock::now();
auto dtn=chrono::duration_cast<chrono::milliseconds>(tend-lastFrameTime);
int64 x=frameTime-dtn.count();
lastFrameTime=mClock::now();
return ((x>0) ? x : 0);
    }
}
