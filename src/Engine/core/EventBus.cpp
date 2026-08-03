#include "EventBus.h"

namespace LANE
{
    void EventBus::Publish(EventData data)
    {
        std::lock_guard<std::mutex> guard(eventQueueMtx);
        eventSystemQueue.emplace_back(data);
    }

    void EventBus::Publish(LayerData data)
    {
        std::lock_guard<std::mutex> guard(layerQueueMtx);
        layerSystemQueue.emplace_back(data);
    }

    std::vector<EventData> EventBus::ReadEventQueue()
    {
        std::lock_guard<std::mutex> guard(eventQueueMtx);
        std::vector<LANE::EventData> ret = std::move(eventSystemQueue);
        return ret;
    }

    std::vector<LayerData> EventBus::ReadLayerQueue()
    {
        std::lock_guard<std::mutex> guard(layerQueueMtx);
        std::vector<LANE::LayerData> ret = std::move(layerSystemQueue);
        return ret;
    }

} // namespace LANE
