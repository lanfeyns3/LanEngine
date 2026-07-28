#pragma once

#include "LayerSystem.h"
#include "WindowSystem.h"

#include <string>
#include <utility>

namespace LANE
{
    class Application
    {
    public:
        void run();
    public:
        LayerSystem layers;
        WindowSystem windows;
    public:
        bool running = true;
    public:
        std::string appName = "LANE";
    };

    class ApplicationBuilder
    {
    public:
        ApplicationBuilder* set_name(const char* name)
        {
            app.appName = name;
            return this;
        };

        ApplicationBuilder* append_layers(std::vector<Layer*> layers)
        {
            for (auto& layer : layers)
            {
                app.layers.AppendLayer(layer);
            }
            return this;
        }

        ApplicationBuilder* add_window(uint32_t width, uint32_t height, const char* name = nullptr)
        {
            const char* title = (name == nullptr) ? app.appName.c_str() : name;
            app.windows.CreateWindow(width,height,title);
            return this;
        }

        Application build() {
            return std::move(app);
        }

    public:
        Application app;
    };
}