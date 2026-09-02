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
          windows(Windows)
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

        ImGui::SetCurrentContext(m_contexts[window]);

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        layers.UpdateImgui();

        glClearColor(
            0.1f,
            0.1f,
            0.1f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);

        auto objects =
            scenes.View<Components::Renderer,Components::Transform,Components::Mesh>(
                scenes.GetCurrentScene()
            );
            
        for (auto object : objects)
        {
            auto asset = assets.GetAsset<ShaderAsset>(69);

            if (!asset)
                continue;
        
            glUseProgram(asset->shaderProgram);
        
            auto& transform = scenes.GetComponent<Components::Transform>(
                scenes.GetCurrentScene(),
                object
            );
        
            glm::mat4 model = glm::mat4(1.0f);
        
            model = glm::translate(
                model,
                transform.position
            );
        
            model = glm::rotate(
                model,
                glm::radians(transform.rotation.z),
                glm::vec3(0.0f, 0.0f, 1.0f)
            );
        
            model = glm::scale(
                model,
                transform.scale
            );
        
            glUniformMatrix4fv(
                glGetUniformLocation(asset->shaderProgram, "model"),
                1,
                GL_FALSE,
                glm::value_ptr(model)
            );

            auto& mesh = scenes.GetComponent<Components::Mesh>(
                scenes.GetCurrentScene(),
                object
            );

            if (m_vaos[window].find(mesh.uuid) == m_vaos[window].end())
                m_vaos[window][mesh.uuid].Create(mesh.vbo,mesh.ebo);

            VAO& vao = m_vaos[window][mesh.uuid];
        
            vao.Load();
            glDrawElements(GL_TRIANGLES, mesh.indiceCount, GL_UNSIGNED_INT, 0);
            vao.Unload();
        }


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

        assets.LoadAsset<ShaderAsset>(
            69,
            file,
            windows.GetWindow("ManorEngineRendererLoader").first
        );
    }

    void Renderer::RegisterWindowImgui(GLFWwindow *window)
    {
        IMGUI_CHECKVERSION();
        ImGuiContext* context = ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        m_contexts[window] = context;
    }
}