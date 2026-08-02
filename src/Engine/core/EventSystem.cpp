#include "EventSystem.h"

namespace LANE
{
    void EventSystem::ReadBus()
    {
        auto events = m_eventBus.ReadEventQueue();
        for (auto& event : events)
        {
            if (event.eventType == EventType::Shutdown)
            {
                m_events[event.eventType].emplace_back(new Shutdown());
            }
        }
    }

    void EventSystem::PollEvents()
    {
        for (auto& event : m_events) 
        {
            if (event.first == EventType::Shutdown)
            {
                extern bool running;
                running = false;
                break;
            }
            
        }
        m_events.clear();
    }

} // namespace LANE
