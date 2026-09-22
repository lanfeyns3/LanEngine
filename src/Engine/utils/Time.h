#pragma once

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

namespace LANE
{
    enum class TimeUnit
    {
        NanoSeconds,
        MiliSeconds,
        Seconds,
        Minutes
    };

    class Time
    {
    public:
        void Start();
        void Stop();

        float Get(TimeUnit unit = TimeUnit::NanoSeconds);
    private:
        std::chrono::steady_clock::time_point past;
    };

    template<class Rep, class Period>
    void SleepThreadFor(std::chrono::duration<Rep, Period> duration)
    {
        std::this_thread::sleep_for(duration);
    }
} // namespace LANE
