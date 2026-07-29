#include "Renderer.h"

namespace LANE
{
    void Renderer::SetGraphics(GraphicsBit graphics)
    {
        if (graphics == GraphicsBit::Vulkan)
        {
            backend = new VulkanRendererBackend();
            backend->Create();
        }
    }

    void Renderer::RenderScene()
    {
        backend->Render();
    }

    void Renderer::AddWindow(GLFWwindow *window)
    {
        backend->AddWindow(window);
    }

} // namespace LANE
