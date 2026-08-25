#include "Renderer.h"
#include <fstream>
#include <iostream>

namespace LANE
{
    std::vector<char> readFile(const char* filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file)
            throw std::runtime_error("failed to open file");

        size_t size = file.tellg();
        std::vector<char> buffer(size);

        file.seekg(0);
        file.read(buffer.data(), size);

        return buffer;
    }

    Renderer::Renderer(
        AssetSystem& Assets,
        SceneManager& Scenes,
        LayerSystem& Layers,
        WindowSystem& Windows)
        : assets(Assets),
          scenes(Scenes),
          layers(Layers),
          windows(Windows)
    {
        glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
        windows.CreateWindow(1280,720,"ManorEngineRendererLoader");
    }

    void Renderer::RenderScene(GLFWwindow* window)
    {
        glfwSwapBuffers(window);
    }

    void Renderer::CreateShader(std::string path)
    {

    }

} // namespace LANE