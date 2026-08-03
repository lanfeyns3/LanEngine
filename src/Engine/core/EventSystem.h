#pragma once

#include <unordered_map>
#include <vector>
#include "Events.h"
#include "EventBus.h"

#include "Layer.h"

namespace LANE
{
    
    class EventSystem
    {
    public:
        EventSystem(EventBus& eventBus) 
            : m_eventBus(eventBus)
        {

        }

        void ReadBus();
        void PollEvents();
        void Subscribe(size_t layer, EventType event);
    private:
        std::unordered_map<EventType,std::vector<Event*>> m_events;
        std::unordered_map<EventType,std::vector<size_t>> m_subscribers;
        EventBus& m_eventBus;
    };
} // namespace LANE
