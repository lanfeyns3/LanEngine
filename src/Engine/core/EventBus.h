#pragma once

#include <vector>
#include <stdint.h>

#include <mutex>
#include <utility>

#include "Events.h"

namespace LANE
{
    struct EventData
    {
        EventType eventType;
        void* data;
    };

    class EventBus
    {
    public:
        EventBus() = default;

        void Publish(EventData data);

        std::vector<EventData> ReadEventQueue();

    private:
        std::vector<EventData> eventSystemQueue;
        std::mutex eventQueueMtx;
    };
} // namespace LANE