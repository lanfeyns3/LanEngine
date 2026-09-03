#pragma once

#include "Layer.h"

#include <unordered_map>
#include <mutex>
#include <utility>

namespace LANE
{
    class LayerSystem
    {
    public:
        void AppendLayer(Layer*& layer,size_t id)
        {
            std::lock_guard<std::mutex> lock(layerMutex);
            m_layers[id] = layer;
        }

        void UpdateLayers(float dt)
        {
            std::lock_guard<std::mutex> lock(layerMutex);
            for (auto layer : m_layers)
            {
                layer.second->Update(dt);
            }
        }

        void UpdateImgui()
        {
            std::lock_guard<std::mutex> lock(layerMutex);
            for (auto layer : m_layers)
            {
                layer.second->ImGuiUpdate();
            }
        }

        void PingEvent(size_t layer,EventType typeE, Event* event)
        {
            std::lock_guard<std::mutex> lock(layerMutex);
            m_layers[layer]->OnEvent(typeE,event);
        }

    private:
        std::unordered_map<size_t,Layer*> m_layers;
        mutable std::mutex layerMutex;
    };
} // namespace LANE