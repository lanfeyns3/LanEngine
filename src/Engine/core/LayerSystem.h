#pragma once

#include "Layer.h"

#include <vector>
#include <mutex>
#include <utility>

namespace LANE
{
    class LayerSystem
    {
    public:
        LayerSystem() = default;

        void AppendLayer(Layer*& layer)
        {
            std::lock_guard<std::mutex> lock(layerMutex);
            m_layers.emplace_back(layer);
        }

        void UpdateLayers()
        {
            std::lock_guard<std::mutex> lock(layerMutex);
            for (auto* layer : m_layers)
            {
                layer->Update();
            }
        }

    private:
        std::vector<Layer*> m_layers;
        mutable std::mutex layerMutex;
    };
} // namespace LANE