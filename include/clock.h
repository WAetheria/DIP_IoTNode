#ifndef MY_CLOCK_H
#define MY_CLOCK_H

#include <Arduino.h>
#include <time.h>

class Timer
{
private:
    unsigned long startTime;
    unsigned long endTime;
    unsigned long timeout;
public:
    Timer();
    Timer(unsigned long timeoutInMs);
    virtual ~Timer();

    void updateTimer();
    void resetTimer();
    void setTimeout(unsigned long timeoutInMs);
    bool timedOut();
    unsigned long getDuration();
};

struct TicToc{
    unsigned long ticStartTime = 0;
    unsigned long tocEndTime   = 0;

    static TicToc& createTicTocTimer(){
        static TicToc tictoc_timer;
        return tictoc_timer;
    }
    
    TicToc(TicToc const&) = delete;
    TicToc(TicToc&&) = delete;

    private:
        TicToc(){};
};

void tic();
void toc();

bool setupTime();
bool SetupTimeAttempts(int count);
String getTime();

#endif