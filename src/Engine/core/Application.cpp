#include "Application.h"

namespace LANE
{
    void Application::run()
    {
        while (running)
        {
            glfwPollEvents();
            layers.UpdateLayers();
        }
        
    }
}