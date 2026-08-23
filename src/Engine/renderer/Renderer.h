#pragma once

#include <volk.h>
#include <VkBootstrap.h>
#include <vk_mem_alloc.h>

#include <GLFW/glfw3.h>
#include <unordered_map>

#include "core/AssetSystem.h"
#include "core/SceneManager.h"

namespace LANE
{
    struct VKWindow
    {
        VkSurfaceKHR surface;
        vkb::Swapchain vkbSwapchain;
        VkRenderPass renderPass;
        std::vector<VkFramebuffer> framebuffers;

        std::vector<VkCommandBuffer> commandBuffers;

        VkCommandPool commandPool;

        VkSemaphore imageAvailable;
        VkSemaphore renderFinished;
        VkFence inFlight;
    };

    class Renderer
    {
    public:
        Renderer(AssetSystem& Assets,SceneManager& Scenes);
    public:
        void RenderScene(GLFWwindow* window);
        void AddWindow(GLFWwindow* window);
        void CreateShader(std::string path);
    private:
        AssetSystem& assets;
        SceneManager& scenes;
    private:
        vkb::Instance vkbInstance;
        vkb::PhysicalDevice vkbPDevice;
        vkb::Device vkbDevice;

        VkQueue graphicsQueue, presentQueue;
        VkBuffer vertexBuffer = VK_NULL_HANDLE;

        VkRenderPass renderPass = VK_NULL_HANDLE;

        VmaAllocator allocator;

        std::vector<VKWindow> windows;
        std::unordered_map<uint64_t, VkPipeline> pipelines;
        std::unordered_map<uint64_t, VkPipelineLayout> pipelineLayouts;
    };
    
} // namespace LANE
