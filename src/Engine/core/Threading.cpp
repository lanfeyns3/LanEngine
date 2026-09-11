#include "Threading.h"

namespace LANE
{
    void Threading::AddThread(std::function<void()> worker)
    {
        threads.emplace_back(worker);
    }

    void Threading::JoinThreads()
    {
        for (auto& thread : threads)
        {
            thread.join();
        }
    }
}