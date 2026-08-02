#pragma once

#include <stdint.h>

namespace LANE 
{
    enum class EventType : uint16_t
    {
        None,
        Shutdown
    };

    struct Event
    {
        
    };

    struct Shutdown : public Event
    {

    };
} // namespace LANE
