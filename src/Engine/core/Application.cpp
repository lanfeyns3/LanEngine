#include "Application.h"

bool running = true;

namespace LANE
{
    void Application::run()
    {
        frameTime.Start();
        prevFrameTime = frameTime.Get();
        while (running)
        {
            glfwPollEvents();
            eventSystem.PollEvents();

            float currentFrameTime = frameTime.Get(TimeUnit::Seconds);
            float dt = prevFrameTime - currentFrameTime;
            prevFrameTime = currentFrameTime;

            layers.UpdateLayers(dt);

            renderer.RenderScene(windows.GetMainWindow());
        }

        threads.JoinThreads();
        
    }
}