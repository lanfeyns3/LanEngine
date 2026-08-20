#pragma once

#include <unordered_map>
#include <vector>
#include "Events.h"

#include "Layer.h"
#include "LayerSystem.h"

namespace LANE
{
    
    class EventSystem
    {
    public:
        EventSystem(LayerSystem& layers)
            : m_layers(layers)
        {}
        void PollEvents();
        void Subscribe(size_t layer, EventType event);
        void AddEvent(EventType type,Event* event);
    private:
        std::unordered_map<EventType,std::vector<Event*>> m_events;
        std::unordered_map<EventType,std::vector<size_t>> m_subscribers;
        LayerSystem& m_layers;
    };
} // namespace LANE
