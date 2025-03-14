#ifndef _TIMER_H
#define _TIMER_H
#include <SDL2/SDL.h>

class Timer {

private:

    static Timer* sInstance;

    unsigned int mStartTicks;
    unsigned int mElapsedTicks;
    float mDeletaTime;
    float mTimeScale;

public:

    static Timer* Instance();
    static void Release();

    void Reset();
    float DeltaTime();

    void TimeScale(float t);
    float TimeScale();

    void Update();

private:

    Timer();
    ~Timer();
};

#endif