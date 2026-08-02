#pragma once

#include <unordered_map>
#include <vector>
#include "Events.h"
#include "EventBus.h"

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
    private:
        std::unordered_map<EventType,std::vector<Event*>> m_events;
        EventBus& m_eventBus;
    };
} // namespace LANE
