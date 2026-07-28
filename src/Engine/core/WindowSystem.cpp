#include "WindowSystem.h"

namespace LANE
{
    void WindowSystem::CreateWindow(uint32_t width, uint32_t height, const char *name)
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
        m_windows.emplace_back(glfwCreateWindow(width,height,name,NULL,NULL));
    }

} // namespace LANE
