#include "Application.h"

bool running = true;

namespace LANE
{
    void Application::run()
    {
        frameTime.Start();
        prevFrameTime = frameTime.Get();

        threads.AddThread([this]() {
            constexpr float fixedDt = 1.0f / 60.0f;

            while (running)
            {
                if (physics.IsPhysicsEnabled())
                    physics.UpdatePhysics(fixedDt, this->layers);
            
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(16)
                );
            }
        });

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