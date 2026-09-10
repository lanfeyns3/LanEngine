#include "WindowSystem.h"
#include <iostream>
#include <cstring>

namespace LANE
{
    GLFWwindow* WindowSystem::CreateWindow(uint32_t width, uint32_t height, const char *name)
    {
        glfwInit();
        
        if (strcmp(name, "ManorEngineRendererLoader") == 0)
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        else
            glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        GLFWwindow* window = nullptr;
        if (strcmp(name, "ManorEngineRendererLoader") == 0)
            window = glfwCreateWindow(width, height, name, NULL, NULL);
        else
            window = glfwCreateWindow(width, height, name, NULL, m_sharedContext);

        m_windows.push_back(window);

        glfwMakeContextCurrent(window);

        if (!m_sharedContext)
        {
            m_sharedContext = window;
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        }
            
        glfwSetWindowUserPointer(window, this);
        glfwSetKeyCallback(window, key_callback);

        return window;
    }

    GLFWwindow *WindowSystem::GetWindow(uint8_t windowID)
    {
        if (windowID < m_windows.size() && m_windows.at(windowID) != nullptr)
        {
            return m_windows.at(windowID);
        }
        return nullptr;
    }

    std::pair<GLFWwindow*, uint8_t> WindowSystem::GetWindow(const char* windowName)
    {
        for (int i = 0; i < m_windows.size(); i++)
        {
            GLFWwindow* window = m_windows.at(i);
            if (strcmp(glfwGetWindowTitle(window), windowName) == 0)
            {
                return std::make_pair(window, i);
            }
        }
        return std::make_pair(nullptr, 255);
    }

    void WindowSystem::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        auto* self = static_cast<WindowSystem*>(glfwGetWindowUserPointer(window));

        if (self)
            self->keyCallback(window, key, scancode, action, mods);
    }

    void WindowSystem::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            KeyEvent* newEvent = new KeyEvent();
            newEvent->key = key;
            newEvent->type = KeyEventType::KeyStroke;
            m_events.AddEvent(EventType::Key, (Event*)newEvent);
        }
        else if (action == GLFW_RELEASE)
        {
            KeyEvent* newEvent = new KeyEvent();
            newEvent->key = key;
            newEvent->type = KeyEventType::KeyReleased;
            m_events.AddEvent(EventType::Key, (Event*)newEvent);
        }
    }
}