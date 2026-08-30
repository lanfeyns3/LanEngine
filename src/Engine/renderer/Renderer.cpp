#include "Renderer.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

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
          windows(Windows),
          vao(0),
          vbo(0)
    {
        GLFWwindow* window = windows.CreateWindow(
            1280,
            720,
            "ManorEngineRendererLoader"
        );

        glfwMakeContextCurrent(NULL);
    }


    void Renderer::RenderScene(GLFWwindow* window)
    {
        glfwMakeContextCurrent(window);

        if (vao == 0 || vbo == 0)
        {
            float vertices[] = {
                 0.0f,  0.5f,
                -0.5f, -0.5f,
                 0.5f, -0.5f
            };

            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(vertices),
                vertices,
                GL_STATIC_DRAW
            );

            glVertexAttribPointer(
                0,
                2,
                GL_FLOAT,
                GL_FALSE,
                2 * sizeof(float),
                nullptr
            );

            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        // --------------------------------------------------
        // Start ImGui frame
        // --------------------------------------------------

        ImGui::SetCurrentContext(m_contexts[window]);

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        // --------------------------------------------------
        // Your ImGui/UI code
        // --------------------------------------------------

        layers.UpdateImgui();

        // --------------------------------------------------
        // Render scene
        // --------------------------------------------------

        glClearColor(
            0.1f,
            0.1f,
            0.1f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);

        auto objects =
            scenes.View<Components::Renderer>(
                scenes.GetCurrentScene()
            );

        if (objects.size() != 0)
        {
            auto asset = assets.GetAsset<ShaderAsset>(69);

            if (asset)
                glUseProgram(asset->shaderProgram);

            glBindVertexArray(vao);

            glDrawArrays(
                GL_TRIANGLES,
                0,
                3
            );

            glBindVertexArray(0);
        }

        // --------------------------------------------------
        // Render ImGui on top of the scene
        // --------------------------------------------------

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(
            ImGui::GetDrawData()
        );

        glfwSwapBuffers(window);
    }


    void Renderer::CreateShader(std::string path)
    {
        std::ifstream f(path);
        json file = json::parse(f);

        assets.LoadAsset<ShaderAsset>(69, file, windows.GetWindow("ManorEngineRendererLoader").first);
    }
    void Renderer::RegisterWindowImgui(GLFWwindow *window)
    {
        IMGUI_CHECKVERSION();
        ImGuiContext* context = ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window,true);
        ImGui_ImplOpenGL3_Init("#version 330");

        m_contexts[window] = context;

    }
} // namespace LANE