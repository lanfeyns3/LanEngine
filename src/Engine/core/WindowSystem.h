#pragma once

#include <GLFW/glfw3.h>

#include <vector>
#include <stdint.h>

namespace LANE
{

    class WindowSystem
    {
    public:
        GLFWwindow* CreateWindow(uint32_t width, uint32_t height, const char* name);
    private:
        std::vector<GLFWwindow*> m_windows;
    };
} // namespace LANE
