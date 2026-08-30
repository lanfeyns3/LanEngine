#pragma once

#include "LayerSystem.h"
#include "WindowSystem.h"
#include "EventSystem.h"
#include "renderer/Renderer.h"
#include "AssetSystem.h"
#include "SceneManager.h"

#include <string>
#include <vector>

namespace LANE
{
    class Application
    {
    public:
        Application()
            : renderer(assets,scenes,layers,windows), windows(eventSystem),eventSystem(layers)
        {
            scenes.AddScene(0);
        }

        Application& set_name(const char* name)
        {
            appName = name;
            return *this;
        }

        Application& append_layers(const std::unordered_map<size_t,Layer*>newLayers)
        {
            for (auto layer : newLayers)
            {
                layers.AppendLayer(layer.second,layer.first);
            }

            return *this;
        }
        
        Application& subscribe_layers(const std::unordered_map<size_t,std::vector<EventType>>& events)
        {
            for (auto entry : events)
            {
                for (auto event : entry.second)
                {
                    eventSystem.Subscribe(entry.first,event);
                }
                
            }
            return *this;
        }

        Application& add_window(uint32_t width, uint32_t height, const char* name = nullptr)
        {
            const char* title = name ? name : appName.c_str();

            windows.CreateWindow(width, height, title);
            auto pair = windows.GetWindow(title);
            windows.SetMainWindow(pair.second);
            renderer.RegisterWindowImgui(pair.first);

            return *this;
        }

        Application& build()
        {
            return *this;
        }

        void run();

    public:
        LayerSystem layers;
        WindowSystem windows;
        Renderer renderer;
        EventSystem eventSystem;
        AssetSystem assets;
        SceneManager scenes;

        std::string appName = "LANE";
    };
}