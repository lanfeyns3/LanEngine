#include "EventSystem.h"

namespace LANE
{
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

            auto& subscribers = m_subscribers[event.first];

            if (subscribers.empty() == false)
            {
                for (auto& lEvent : event.second)
                {
                    for (size_t subscriber : subscribers)
                    {
                        m_layers.PingEvent(subscriber,event.first,lEvent);
                    }
                }
                
            }
            
        }
        m_events.clear();
    }

    void EventSystem::Subscribe(size_t layer, EventType event)
    {
        m_subscribers[event].emplace_back(layer);
    }

    void EventSystem::AddEvent(EventType type, Event *event)
    {
        m_events[type].emplace_back(event);
    }

} // namespace LANE
