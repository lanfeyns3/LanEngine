#pragma once

#include "backends/VulkanBackend.h"

namespace LANE
{
    enum GraphicsBit
    {
        Vulkan
    };

    class Renderer
    {
    public:
        void SetGraphics(GraphicsBit graphics);
        void RenderScene();
        void AddWindow(GLFWwindow* window);

    private:
        RendererBackend* backend;
    };
    
} // namespace LANE
