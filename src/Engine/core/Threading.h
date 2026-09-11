#pragma once

#include <vector>
#include <functional>
#include <thread>

namespace LANE
{
    class Threading
    {
    public:
        void AddThread(std::function<void()> worker);
        void JoinThreads();
    private:
        std::vector<std::thread> threads;
    };
}