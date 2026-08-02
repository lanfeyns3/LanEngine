#pragma once

#include <GLFW/glfw3.h>

#include <vector>
#include <stdint.h>

#include "EventBus.h"

namespace LANE
{

    class WindowSystem
    {
    public:
        WindowSystem(EventBus& eventBus)
            : m_eventBus(eventBus)
        {}

        GLFWwindow* CreateWindow(uint32_t width, uint32_t height, const char* name);
    private:
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    private:
        std::vector<GLFWwindow*> m_windows;
        EventBus& m_eventBus;
    };
} // namespace LANE
