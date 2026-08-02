#include "EventBus.h"

namespace LANE
{
    void EventBus::Publish(EventData data)
    {
        std::lock_guard<std::mutex> guard(eventQueueMtx);
        eventSystemQueue.emplace_back(data);
    }

    std::vector<EventData> EventBus::ReadEventQueue()
    {
        std::lock_guard<std::mutex> guard(eventQueueMtx);
        return eventSystemQueue;
    }

} // namespace LANE
