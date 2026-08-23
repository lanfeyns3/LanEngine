#define VMA_IMPLEMENTATION

#include "Renderer.h"
#include <fstream>
#include <iostream>

namespace LANE
{
    std::vector<char> readFile(const char* filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file)
            throw std::runtime_error("failed to open file");

        size_t size = file.tellg();

        std::vector<char> buffer(size);

        file.seekg(0);
        file.read(buffer.data(), size);

        return buffer;
    }

    

    Renderer::Renderer(AssetSystem& Assets,SceneManager& Scenes)
        : assets(Assets), scenes(Scenes)
    {
        volkInitialize();

        {
            vkb::InstanceBuilder builder;

            auto result = builder
                .set_app_name("Custom Vulkan Engine")
                .request_validation_layers()
                .use_default_debug_messenger()
                .build();

            vkbInstance = result.value();

            volkLoadInstance(vkbInstance.instance);
        }
    }

    void Renderer::RenderScene(GLFWwindow* window)
    {
        if (vertexBuffer == VK_NULL_HANDLE)
        {
            float vertexData[] = {
                 0.0f, -0.5f,
                 0.5f,  0.5f,
                -0.5f,  0.5f
            };

            VmaAllocation vertexBufferAllocation;

            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = sizeof(vertexData);
            bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VmaAllocationCreateInfo allocInfo{};
            allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;


            vmaCreateBuffer(
                allocator,
                &bufferInfo,
                &allocInfo,
                &vertexBuffer,
                &vertexBufferAllocation,
                nullptr
            );


            void* data;
            vmaMapMemory(allocator, vertexBufferAllocation, &data);
            memcpy(data, vertexData, sizeof(vertexData));
            vmaUnmapMemory(allocator, vertexBufferAllocation);
        }

        if (scenes.View<Components::Renderer>(scenes.GetCurrentScene()).size() != 0)
        {
            VKWindow& renderWindow = windows[0];
            vkWaitForFences(vkbDevice.device, 1, &renderWindow.inFlight, VK_TRUE, UINT64_MAX);
            vkResetFences(vkbDevice.device, 1, &renderWindow.inFlight);

            uint32_t imageIndex;
            vkAcquireNextImageKHR(
                vkbDevice.device,
                renderWindow.vkbSwapchain.swapchain,
                UINT64_MAX,
                renderWindow.imageAvailable,
                VK_NULL_HANDLE,
                &imageIndex);
            
            VkCommandBuffer cmd = renderWindow.commandBuffers[imageIndex];
            vkResetCommandBuffer(cmd, 0);
            
            VkCommandBufferBeginInfo begin{};
            begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            vkBeginCommandBuffer(cmd, &begin);
            
            VkClearValue clear{};
            clear.color =
            {
                {0.1f,0.1f,0.1f,1.0f}
            };
            
            VkRenderPassBeginInfo rp{};
            rp.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            rp.renderPass = renderPass;
            rp.framebuffer = renderWindow.framebuffers[imageIndex];
            rp.renderArea.extent = {1280, 720};
            rp.clearValueCount = 1;
            rp.pClearValues = &clear;
            
            vkCmdBeginRenderPass(cmd, &rp, VK_SUBPASS_CONTENTS_INLINE);
            
            vkCmdBindPipeline(
                cmd,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                pipelines[15482739461592837462]);
            
            
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(
                cmd,
                0,
                1,
                &vertexBuffer,
                offsets
            );

            vkCmdDraw(
                cmd,
                3,
                1,
                0,
                0);

            vkCmdEndRenderPass(cmd);
            vkEndCommandBuffer(cmd);
            
            VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            VkSubmitInfo submit{};
            submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submit.waitSemaphoreCount = 1;
            submit.pWaitSemaphores = &renderWindow.imageAvailable;
            submit.pWaitDstStageMask = &waitStage;
            submit.commandBufferCount = 1;
            submit.pCommandBuffers = &cmd;
            submit.signalSemaphoreCount = 1;
            submit.pSignalSemaphores = &renderWindow.renderFinished;
            
            vkQueueSubmit(graphicsQueue, 1, &submit, renderWindow.inFlight);
            
            VkPresentInfoKHR present{};
            present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            present.waitSemaphoreCount = 1;
            present.pWaitSemaphores = &renderWindow.renderFinished;
            present.swapchainCount = 1;
            present.pSwapchains = &renderWindow.vkbSwapchain.swapchain;
            present.pImageIndices = &imageIndex;
            
            vkQueuePresentKHR(presentQueue, &present);
        }
    }

    void Renderer::AddWindow(GLFWwindow *window)
    {
        auto& vkWindow = windows.emplace_back();

        glfwCreateWindowSurface(vkbInstance.instance,window,nullptr,&vkWindow.surface);

        if (vkbPDevice == nullptr && vkbDevice == nullptr)
        {
            {
                vkb::PhysicalDeviceSelector selector(vkbInstance);

                auto result = selector
                    .prefer_gpu_device_type()
                    .set_surface(vkWindow.surface)
                    .add_required_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME)
                    .require_present()
                    .select();

                vkbPDevice = result.value();

                printf(
                    "Selected GPU: %s\n",
                    vkbPDevice.properties.deviceName
                );
            }

            {
                auto builder = vkb::DeviceBuilder(vkbPDevice);
                auto ret = builder.build();

                vkbDevice = ret.value();

                auto grapgics_ret = vkbDevice.get_queue(vkb::QueueType::graphics);
                graphicsQueue = grapgics_ret.value();
                auto present_ret = vkbDevice.get_queue(vkb::QueueType::present);
                presentQueue = present_ret.value();
            }
            volkLoadDevice(vkbDevice.device);
        }

        {
            auto builder = vkb::SwapchainBuilder(vkbDevice);
            auto ret =
                vkb::SwapchainBuilder(vkbDevice)
                    .set_desired_format(
                        VkSurfaceFormatKHR{
                            VK_FORMAT_B8G8R8A8_UNORM,
                            VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
                        })
                    .set_desired_present_mode(
                        VK_PRESENT_MODE_MAILBOX_KHR)
                    .set_desired_extent(1280,720)
                    .build();

            vkWindow.vkbSwapchain = ret.value();
        }

        {
            VkFormat swapFormat =
                vkWindow.vkbSwapchain.image_format;

            auto swapImages =
                vkWindow.vkbSwapchain.get_images().value();

            auto swapViews =
                vkWindow.vkbSwapchain.get_image_views().value();
            
            VkAttachmentDescription color{};
            color.format = swapFormat;
            color.samples = VK_SAMPLE_COUNT_1_BIT;
            color.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            color.initialLayout =
                VK_IMAGE_LAYOUT_UNDEFINED;
            color.finalLayout =
                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


            VkAttachmentReference ref{};
            ref.attachment = 0;
            ref.layout =
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


            VkSubpassDescription sub{};
            sub.pipelineBindPoint =
                VK_PIPELINE_BIND_POINT_GRAPHICS;
            sub.colorAttachmentCount = 1;
            sub.pColorAttachments = &ref;


            if (renderPass == VK_NULL_HANDLE)
            {
                VkRenderPassCreateInfo rp{};
                rp.sType =
                    VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

                rp.attachmentCount = 1;
                rp.pAttachments = &color;

                rp.subpassCount = 1;
                rp.pSubpasses = &sub;

                vkCreateRenderPass(
                    vkbDevice.device,
                    &rp,
                    nullptr,
                    &renderPass);
            }
        }

        {
            auto swapViews =
                vkWindow.vkbSwapchain.get_image_views().value();

            vkWindow.framebuffers.resize(swapViews.size());

            for (size_t i = 0; i < swapViews.size(); i++)
            {
                VkImageView attachments[] =
                {
                    swapViews[i]
                };

                VkFramebufferCreateInfo fb{};
                fb.sType =
                    VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

                fb.renderPass = renderPass;

                fb.attachmentCount = 1;
                fb.pAttachments = attachments;

                fb.width = 1280;
                fb.height = 720;
                fb.layers = 1;


                VkResult result =
                    vkCreateFramebuffer(
                        vkbDevice.device,
                        &fb,
                        nullptr,
                        &vkWindow.framebuffers[i]);

                if (result != VK_SUCCESS)
                {
                    throw std::runtime_error(
                        "failed to create framebuffer");
                }
            }

            printf("Created %zu framebuffers\n",
                   vkWindow.framebuffers.size());
        }

        {
            VkCommandPoolCreateInfo pool{};
            pool.sType =
                VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

            pool.queueFamilyIndex =
                vkbDevice.get_queue_index(
                    vkb::QueueType::graphics
                ).value();
            
            
            vkCreateCommandPool(
                vkbDevice.device,
                &pool,
                nullptr,
                &vkWindow.commandPool);
        }

        {
            vkWindow.commandBuffers.resize(vkWindow.framebuffers.size());


            VkCommandBufferAllocateInfo alloc{};
            alloc.sType =
                VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

            alloc.commandPool = vkWindow.commandPool;
            alloc.level =
                VK_COMMAND_BUFFER_LEVEL_PRIMARY;

            alloc.commandBufferCount =
                vkWindow.commandBuffers.size();


            vkAllocateCommandBuffers(
                vkbDevice.device,
                &alloc,
                vkWindow.commandBuffers.data());
        }

        {
            VkSemaphoreCreateInfo sem{};
            sem.sType =
                VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;


            vkCreateSemaphore(
                vkbDevice.device,
                &sem,
                nullptr,
                &vkWindow.imageAvailable);
            
            
            vkCreateSemaphore(
                vkbDevice.device,
                &sem,
                nullptr,
                &vkWindow.renderFinished);
            
            
            
            VkFenceCreateInfo fence{};
            fence.sType =
                VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            
            fence.flags =
                VK_FENCE_CREATE_SIGNALED_BIT;
            
            
            vkCreateFence(
                vkbDevice.device,
                &fence,
                nullptr,
                &vkWindow.inFlight);
        }

        {
            VmaVulkanFunctions vulkanFunctions{};

            vulkanFunctions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
            vulkanFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;


            VmaAllocatorCreateInfo allocatorInfo{};
            allocatorInfo.physicalDevice = vkbPDevice.physical_device;
            allocatorInfo.device = vkbDevice.device;
            allocatorInfo.instance = vkbInstance.instance;

            allocatorInfo.pVulkanFunctions = &vulkanFunctions;

            vmaCreateAllocator(&allocatorInfo, &allocator);
        }
    }

    void Renderer::CreateShader(std::string path)
    {
        std::fstream f(path);

        nlohmann::json file = nlohmann::json::parse(f);
        auto shaderAsset = assets.LoadAsset<ShaderAsset>(4,file,vkbDevice.device);

            
        VkPipelineShaderStageCreateInfo stages[2]{};
            
            
        stages[0].sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            
        stages[0].stage =
        VK_SHADER_STAGE_VERTEX_BIT;
            
        stages[0].module = shaderAsset->vertex;
        stages[0].pName = "main";
            
            
        stages[1].sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            
        stages[1].stage =
        VK_SHADER_STAGE_FRAGMENT_BIT;
            
        stages[1].module = shaderAsset->fragment;
        stages[1].pName = "main";
            
            
        VkPipelineVertexInputStateCreateInfo vertex{};
        vertex.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            
            
        VkPipelineInputAssemblyStateCreateInfo assembly{};
        assembly.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            
        assembly.topology =
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            
            
            
        VkViewport viewport{};
        viewport.width = 1280;
        viewport.height = 720;
        viewport.maxDepth = 1;
            
            
        VkRect2D scissor{};
        scissor.extent =
        {1280,720};
            
            
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            
        viewportState.viewportCount=1;
        viewportState.pViewports=&viewport;
            
        viewportState.scissorCount=1;
        viewportState.pScissors=&scissor;
            
            
            
        VkPipelineRasterizationStateCreateInfo raster{};
        raster.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            
        raster.polygonMode =
        VK_POLYGON_MODE_FILL;
            
        raster.cullMode =
        VK_CULL_MODE_BACK_BIT;
            
        raster.frontFace =
        VK_FRONT_FACE_CLOCKWISE;
            
        raster.lineWidth=1;
            
            
        VkPipelineMultisampleStateCreateInfo msaa{};
        msaa.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            
        msaa.rasterizationSamples =
        VK_SAMPLE_COUNT_1_BIT;
            
            
        VkPipelineColorBlendAttachmentState blend{};
        blend.colorWriteMask =
        0xf;
            
            
        VkPipelineColorBlendStateCreateInfo blendState{};
        blendState.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            
        blendState.attachmentCount=1;
        blendState.pAttachments=&blend;
            
            
            
        VkPipelineLayoutCreateInfo layoutInfo{};
        layoutInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            
        vkCreatePipelineLayout(
            vkbDevice.device,
            &layoutInfo,
            nullptr,
            &pipelineLayouts[file["UUID"]]);
        
        
        
        VkGraphicsPipelineCreateInfo pipe{};
        pipe.sType =
        VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        
        pipe.stageCount=2;
        pipe.pStages=stages;
        
        pipe.pVertexInputState=&vertex;
        pipe.pInputAssemblyState=&assembly;
        pipe.pViewportState=&viewportState;
        pipe.pRasterizationState=&raster;
        pipe.pMultisampleState=&msaa;
        pipe.pColorBlendState=&blendState;
        
        pipe.layout=pipelineLayouts[file["UUID"]];
        pipe.renderPass=renderPass;
        
        vkCreateGraphicsPipelines(
            vkbDevice.device,
            VK_NULL_HANDLE,
            1,
            &pipe,
            nullptr,
            &pipelines[file["UUID"]]);
    }

    

} // namespace LANE
