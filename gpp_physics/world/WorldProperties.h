

#ifndef WORLDPROPERTIES_H
#define WORLDPROPERTIES_H

#include<chrono>

namespace gpp{

class WorldProperties{
public:
int64 fps;
int64 frameTime;
decimal linearDamping;
decimal angularDamping;
decimal currentTime;
decimal timeStep;
decimal radius;
uint64 currentFrame;
vector3d gravity;
vector3d center;

private:

using mClock =std::chrono::high_resolution_clock;
mClock::time_point lastFrameTime;

public:
WorldProperties();
WorldProperties(const WorldProperties& wp);
WorldProperties& operator=(const WorldProperties& wb);
std::string toString()const;
void cleanup();
void toDefault();

private:

void advance(decimal dt);
int64 sleepTime();
friend class gpp_world;
};
}
#endif
