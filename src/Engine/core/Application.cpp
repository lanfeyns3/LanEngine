#include "Application.h"

bool running = true;

namespace LANE
{
    void Application::run()
    {
        while (running)
        {
            glfwPollEvents();

            eventSystem.PollEvents();

            layers.UpdateLayers();

            renderer.RenderScene(windows.GetMainWindow());
        }
        
    }
}