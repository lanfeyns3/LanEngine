#pragma once

#include <volk.h>
#include <VkBootstrap.h>
#include "RendererBackend.h"

namespace LANE
{
    struct VKWindow
    {
        VkSurfaceKHR surface;
        vkb::Swapchain vkbSwapchain;
        VkRenderPass renderPass;
        std::vector<VkFramebuffer> framebuffers;

        VkPipeline pipeline;
        VkPipelineLayout layout;
        std::vector<VkCommandBuffer> commandBuffers;

        VkCommandPool commandPool;

        VkSemaphore imageAvailable;
        VkSemaphore renderFinished;
        VkFence inFlight;
    };
    
    class VulkanRendererBackend : public RendererBackend
    {
    public:
        void Create();
        void AddWindow(GLFWwindow* window);
        void Render();
    private:
        vkb::Instance vkbInstance;
        vkb::PhysicalDevice vkbPDevice;
        vkb::Device vkbDevice;

        VkQueue graphicsQueue, presentQueue;

        std::vector<VKWindow> windows;
    };

} // namespace LANE
