#pragma once

#include <stdint.h>

namespace LANE 
{
    enum class EventType : uint16_t
    {
        None,
        Shutdown,
        Key
    };

    struct Event
    {
        
    };

    enum class KeyEventType
    {
        KeyStroke,
        KeyHeld,
        KeyReleased
    };

    struct KeyEvent : public Event
    {
        KeyEventType type;
        uint32_t key;
    };

    struct ShutdownEvent : public Event
    {

    };
} // namespace LANE
