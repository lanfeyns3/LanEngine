#pragma once

#include "Events.h"
#include <stdint.h>

namespace LANE
{
    class Layer
    {
    public:
        virtual void Update(float dt) = 0;
        virtual void ImGuiUpdate() = 0;
        virtual void OnEvent(EventType eType, Event* event) = 0;
    };
}
