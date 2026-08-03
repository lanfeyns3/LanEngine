#pragma once

#include "Events.h"

namespace LANE
{
    class Layer
    {
    public:
        virtual void Update() = 0;
        virtual void OnEvent(EventType eType, Event* event) = 0;
    };
} // namespace LANE
