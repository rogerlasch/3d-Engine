


#ifndef CLOCKGAME_H
#define CLOCKGAME_H

namespace gpp{

typedef std::chrono::time_point<std::chrono::high_resolution_clock> gtimepoint;

class ClockGame{
private:

float delta;
gtimepoint spoint;
gtimepoint npoint;

public:
ClockGame();
ClockGame(const ClockGame& cg)=delete;
ClockGame& operator=(const ClockGame& cg)=delete;
virtual ~ ClockGame();

inline float getDelta()const{return this->delta;}
double getTotalTime();
void tick();

};
}
#endif
