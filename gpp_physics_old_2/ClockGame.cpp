

#include<chrono>
#include"ClockGame.h"

using namespace std;

namespace gpp{

ClockGame::ClockGame(){
spoint=chrono::high_resolution_clock::now();
npoint=spoint;
tick();
}

ClockGame::~ ClockGame(){
}

double ClockGame::getTotalTime(){
gtimepoint time = chrono::high_resolution_clock::now();

        chrono::duration<double> diff;
        diff = chrono::duration_cast<chrono::microseconds>(time - spoint);

        return diff.count();// / 1000.0f;

}

void ClockGame::tick(){
gtimepoint latestTime = chrono::high_resolution_clock::now();
        auto msec = chrono::duration_cast<chrono::microseconds>(latestTime - npoint);
        npoint = latestTime;
delta = msec.count() / 1000.0f;
}
}
