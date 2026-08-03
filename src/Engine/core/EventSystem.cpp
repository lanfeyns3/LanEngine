#include "EventSystem.h"

namespace LANE
{
    void EventSystem::ReadBus()
    {
        auto events = m_eventBus.ReadEventQueue();
        for (auto& event : events)
        {
            switch (event.eventType)
            {
            case EventType::Key:
            {
                KeyEvent* newEvent = new KeyEvent();
                KeyEvent* cast = (KeyEvent*)event.data;
                newEvent->key = cast->key;
                newEvent->type = cast->type;

                m_events[event.eventType].emplace_back(newEvent);
                break;
            }
            default:
                break;
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

            auto& subscribers = m_subscribers[event.first];

            if (subscribers.empty() == false)
            {
                for (auto& lEvent : event.second)
                {
                    for (size_t subscriber : subscribers)
                    {
                        LayerData data{};
                        data.data = lEvent;
                        data.eventType = event.first;
                        data.layer = subscriber;

                        m_eventBus.Publish(data);
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

} // namespace LANE
