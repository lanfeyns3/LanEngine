#pragma once

#include <chrono>

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
} // namespace LANE
