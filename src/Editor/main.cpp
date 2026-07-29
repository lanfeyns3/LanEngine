#include "core/Application.h"

#include "Layers/EditorLayer.h"

int main()
{
    LANE::ApplicationBuilder* builder = new LANE::ApplicationBuilder();

    std::vector<LANE::Layer*> layers;
    layers.reserve(1);
    layers.emplace_back(new EditorLayer());

    auto app = builder->set_name("Editor")
        ->append_layers(layers)
        ->set_graphics(LANE::GraphicsBit::Vulkan)
        ->add_window(800,600)
        ->build();

    delete builder;

    app.run();
}