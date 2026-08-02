#pragma once

#include "EventBus.h"
#include "LayerSystem.h"
#include "WindowSystem.h"
#include "EventSystem.h"
#include "renderer/Renderer.h"

#include <string>
#include <vector>

namespace LANE
{
    class Application
    {
    public:
        Application()
            : eventSystem(eventBus), windows(eventBus)
        {}

        Application& set_name(const char* name)
        {
            appName = name;
            return *this;
        }

        Application& append_layers(const std::vector<Layer*>& newLayers)
        {
            for (auto* layer : newLayers)
            {
                layers.AppendLayer(layer);
            }

            return *this;
        }

        Application& set_graphics(GraphicsBit graphics)
        {
            renderer.SetGraphics(graphics);
            return *this;
        }

        Application& add_window(uint32_t width, uint32_t height, const char* name = nullptr)
        {
            const char* title = name ? name : appName.c_str();

            renderer.AddWindow(
                windows.CreateWindow(width, height, title)
            );

            return *this;
        }

        Application& build()
        {
            return *this;
        }

        void run();

    public:
        EventBus eventBus;
        LayerSystem layers;
        WindowSystem windows;
        Renderer renderer;
        EventSystem eventSystem;

        std::string appName = "LANE";
    };
}