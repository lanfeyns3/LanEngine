#pragma once

namespace LANE
{
    class Layer
    {
    public:
        virtual void Update() = 0;
        virtual void OnEvent() = 0;
    };
} // namespace LANE
