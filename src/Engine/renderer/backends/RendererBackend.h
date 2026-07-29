#pragma once

#include <GLFW/glfw3.h>

namespace LANE
{
    class RendererBackend
    {
    public:
        virtual void Create() = 0;
        virtual void Render() = 0;
        virtual void AddWindow(GLFWwindow* window) = 0;
    };
} // namespace LANE
