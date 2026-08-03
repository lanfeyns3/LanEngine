#pragma once

#include "Layer.h"

#include <unordered_map>
#include <mutex>
#include <utility>

#include "EventBus.h"

namespace LANE
{
    class LayerSystem
    {
    public:
        LayerSystem(EventBus& eventBus)
            : m_eventBus(eventBus)
        {}

        void AppendLayer(Layer*& layer,size_t id)
        {
            std::lock_guard<std::mutex> lock(layerMutex);
            m_layers[id] = layer;
        }

        void UpdateLayers()
        {
            std::lock_guard<std::mutex> lock(layerMutex);
            for (auto layer : m_layers)
            {
                layer.second->Update();
            }
        }

        void ReadBus()
        {
            std::lock_guard<std::mutex> lock(layerMutex);
            std::vector<LayerData> queue = m_eventBus.ReadLayerQueue();

            for (LayerData data : queue)
            {
                m_layers[data.layer]->OnEvent(data.eventType,data.data);
            }
        }

    private:
        std::unordered_map<size_t,Layer*> m_layers;

        EventBus& m_eventBus;
        mutable std::mutex layerMutex;
    };
} // namespace LANE