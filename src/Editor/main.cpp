#include "core/Application.h"

#include "Layers/EditorLayer.h"

int main()
{

    std::vector<LANE::Layer*> layers;
    layers.reserve(1);
    layers.emplace_back(new EditorLayer());

    LANE::Application app;

    app
        .set_name("My Game")
        .set_graphics(LANE::GraphicsBit::Vulkan)
        .add_window(1280, 720)
        .append_layers(layers)
        .build()
        .run();
}