#include "core/Application.h"

#include "Layers/EditorLayer.h"

int main()
{
    LANE::Application app;

    std::unordered_map<size_t,LANE::Layer*> layers;
    layers.reserve(1);
    layers[typeid(EditorLayer).hash_code()] = new EditorLayer(app);

    std::unordered_map<size_t,std::vector<LANE::EventType>> subscribers;
    subscribers[typeid(EditorLayer).hash_code()] = {LANE::EventType::Key};

    

    app
        .set_name("My Game")
        .add_window(1280, 720)
        .append_layers(layers)
        .subscribe_layers(subscribers)
        .build();
        
    app.renderer.CreateShader("./shaders/shader.shader");
    app.run();
}