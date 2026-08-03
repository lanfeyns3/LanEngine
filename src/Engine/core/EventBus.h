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

    struct LayerData
    {
        size_t layer;
        EventType eventType;
        Event* data;
    };

    class EventBus
    {
    public:
        EventBus() = default;

        void Publish(EventData data);
        void Publish(LayerData data);

        std::vector<EventData> ReadEventQueue();
        std::vector<LayerData> ReadLayerQueue();

    private:
        std::vector<EventData> eventSystemQueue;
        std::vector<LayerData> layerSystemQueue;
        std::mutex eventQueueMtx;
        std::mutex layerQueueMtx;
    };
} // namespace LANE