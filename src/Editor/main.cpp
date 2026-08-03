#include "core/Application.h"

#include "Layers/EditorLayer.h"

int main()
{

    std::unordered_map<size_t,LANE::Layer*> layers;
    layers.reserve(1);
    layers[typeid(EditorLayer).hash_code()] = new EditorLayer();

    std::unordered_map<size_t,std::vector<LANE::EventType>> subscribers;
    subscribers[typeid(EditorLayer).hash_code()] = {LANE::EventType::Key};

    LANE::Application app;

    app
        .set_name("My Game")
        .set_graphics(LANE::GraphicsBit::Vulkan)
        .add_window(1280, 720)
        .append_layers(layers)
        .subscribe_layers(subscribers)
        .build()
        .run();
}