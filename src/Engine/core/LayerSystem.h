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

        // No copying
        LayerSystem(const LayerSystem&) = delete;
        LayerSystem& operator=(const LayerSystem&) = delete;

        // Move constructor
        LayerSystem(LayerSystem&& other) noexcept
        {
            std::lock_guard<std::mutex> lock(other.layerMutex);
            m_layers = std::move(other.m_layers);
        }

        // Move assignment
        LayerSystem& operator=(LayerSystem&& other) noexcept
        {
            if (this != &other)
            {
                // Lock both mutexes without deadlock
                std::scoped_lock lock(layerMutex, other.layerMutex);
                m_layers = std::move(other.m_layers);
            }
            return *this;
        }

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