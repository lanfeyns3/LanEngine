#pragma once

#include <volk.h>
#include <VkBootstrap.h>
#include <GLFW/glfw3.h>

#include "core/AssetSystem.h"

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

    class Renderer
    {
    public:
        Renderer(AssetSystem& Assets);
    public:
        void RenderScene(GLFWwindow* window);
        void AddWindow(GLFWwindow* window);
    private:
        AssetSystem& assets;
    private:
        vkb::Instance vkbInstance;
        vkb::PhysicalDevice vkbPDevice;
        vkb::Device vkbDevice;

        VkQueue graphicsQueue, presentQueue;

        std::vector<VKWindow> windows;
    };
    
} // namespace LANE
