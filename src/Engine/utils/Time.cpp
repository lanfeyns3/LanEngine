#include "Time.h"

namespace LANE
{
    void Time::Start()
    {
        past = std::chrono::high_resolution_clock::now();
    }

    void Time::Stop()
    {

    }

    float Time::Get(TimeUnit unit)
    {
        auto now = std::chrono::high_resolution_clock::now();

        if (unit == TimeUnit::NanoSeconds) {
            return std::chrono::duration<float, std::nano>(now - past).count();
        }
        else if (unit == TimeUnit::MiliSeconds) {
            return std::chrono::duration<float, std::milli>(now - past).count();
        }
        else if (unit == TimeUnit::Seconds) {
            return std::chrono::duration<float>(now - past).count();
        }
    }
}