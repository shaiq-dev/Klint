#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <chrono>
#include <functional>

class Timer
{
    std::thread Thread;
    bool Alive = false;
    long callNumber = -1L;
    long repeat_count = -1L;

    std::chrono::milliseconds interval = std::chrono::milliseconds(0);
    std::function<void(void)> funct = nullptr;

    void sleepAndRun()
    {
        std::this_thread::sleep_for(interval);
        if(Alive)
            Function()();
    }

    void ThreadFunc()
    {
        if(callNumber == Infinite)
            while(Alive)
                sleepAndRun();
        else
            while(repeat_count--)
                sleepAndRun();
    }

    public:
        static const long Infinite = -1L;

        Timer(){}

        Timer(const std::function<void(void)> &f) : funct (f) {}

        Timer(const std::function<void(void)> &f,
              const unsigned long &i,
              const long repeat = Timer::Infinite) : funct (f), interval(std::chrono::milliseconds(i)), callNumber(repeat)
              {}

    void start(bool async = true)
    {
        if(isAlive())
            return;

        Alive = true;
        repeat_count = callNumber;

        if(async)
            Thread = std::thread(ThreadFunc, this);
        else
            this->ThreadFunc();
    }

    void stop()
    {
        Alive = false;
        Thread.join();
    }

    void setFunction(const std::function<void(void)> &f)
    {
        funct = f;
    }

    bool isAlive() const
    {
        return Alive;
    }

    void repeatCount(const long r)
    {
        if(Alive)
            return;

        callNumber = r;
    }

    long getLeftCount() const
    {
        return repeat_count;
    }

    long repeatCount() const
    {
        return callNumber;
    }

    void setInterval(const unsigned long &i)
    {
        if(Alive)
            return;

        interval = std::chrono::milliseconds(i);
    }

    unsigned long Interval() const
    {
        return interval.count();
    }

    const std::function<void(void)> &Function() const
    {
        return funct;
    }


};

#endif

