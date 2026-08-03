#include "WindowSystem.h"

namespace LANE
{
    GLFWwindow* WindowSystem::CreateWindow(uint32_t width, uint32_t height, const char *name)
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
        GLFWwindow* window = m_windows.emplace_back(glfwCreateWindow(width,height,name,NULL,NULL));

        glfwSetWindowUserPointer(window, this);
        glfwSetKeyCallback(window, key_callback);

        return window;
    }

    void WindowSystem::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        auto* self = static_cast<WindowSystem*>(glfwGetWindowUserPointer(window));

        if (self)
            self->keyCallback(window,key,scancode,action,mods);
    }

    void WindowSystem::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            EventData data;
            KeyEvent* keyEvent = new KeyEvent;
            keyEvent->key = key;
            keyEvent->type = KeyEventType::KeyStroke;
            
            data.eventType = EventType::Key;
            data.data = keyEvent;
            m_eventBus.Publish(data);
        }
    }

} // namespace LANE
