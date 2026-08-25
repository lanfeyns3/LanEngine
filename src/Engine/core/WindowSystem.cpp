#include "WindowSystem.h"

namespace LANE
{
    GLFWwindow* WindowSystem::CreateWindow(uint32_t width, uint32_t height, const char *name)
    {
        glfwInit();
        
        GLFWwindow* window = m_windows.emplace_back(glfwCreateWindow(width,height,name,NULL,NULL));

        glfwSetWindowUserPointer(window, this);
        glfwSetKeyCallback(window, key_callback);

        return window;
    }

    void WindowSystem::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) // Front
    {
        auto* self = static_cast<WindowSystem*>(glfwGetWindowUserPointer(window));

        if (self)
            self->keyCallback(window,key,scancode,action,mods);
    }

    void WindowSystem::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) // Actual actions
    {
        if (action == GLFW_PRESS)
        {
            KeyEvent* newEvent = new KeyEvent();
            newEvent->key = key;
            newEvent->type = KeyEventType::KeyStroke;
            m_events.AddEvent(EventType::Key,(Event*)newEvent);
        }
    }

} // namespace LANE
