#include "Renderer.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

struct GPULight
{
    glm::vec3 position;
    float radius;
    float intensity;

    glm::vec3 color;
    float _padding;
};

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

        glGenBuffers(1, &lightUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);

        glBufferData(
            GL_UNIFORM_BUFFER,
            sizeof(GPULight),              // size in bytes
            nullptr,
            GL_DYNAMIC_DRAW
        );

        glBindBuffer(GL_UNIFORM_BUFFER, 0);


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

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);

        glEnable(GL_DEPTH_TEST);

        glClearColor(
            0.1f,
            0.1f,
            0.1f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );

        auto objects =
            scenes.View<
                Components::Renderer,
                Components::Transform,
                Components::Mesh
            >(
                scenes.GetCurrentScene()
            );

        Components::Camera& mainCamera =
            scenes.GetMainCamera();

        /*
         * ----------------------------------------
         * CAMERA
         * ----------------------------------------
         */

        glm::vec3 cameraPosition =
            mainCamera.position;

        glm::vec3 cameraRotation =
            mainCamera.rotation;

        // Build camera rotation.
        glm::mat4 cameraRotationMatrix =
            glm::mat4(1.0f);

        cameraRotationMatrix = glm::rotate(
            cameraRotationMatrix,
            glm::radians(cameraRotation.x),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        cameraRotationMatrix = glm::rotate(
            cameraRotationMatrix,
            glm::radians(cameraRotation.y),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        cameraRotationMatrix = glm::rotate(
            cameraRotationMatrix,
            glm::radians(cameraRotation.z),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );

        /*
         * Camera looks down -Z in OpenGL.
         */

        glm::vec3 forward =
            glm::vec3(
                cameraRotationMatrix *
                glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)
            );

        glm::vec3 up =
            glm::vec3(
                cameraRotationMatrix *
                glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)
            );

        glm::mat4 view =
            glm::lookAt(
                cameraPosition,
                cameraPosition + forward,
                up
            );

        /*
         * ----------------------------------------
         * PROJECTION
         * ----------------------------------------
         */

        float aspect =
            static_cast<float>(width) /
            static_cast<float>(height);

        
        glm::mat4 projection =
            glm::perspective(
                glm::radians(mainCamera.fov),
                aspect,
                mainCamera.nearPlane,
                mainCamera.farPlane
            );

        /*
         * ----------------------------------------
         * OBJECTS
         * ----------------------------------------
         */

        // setup lights. TODO: allow more lights
        GPULight gpuLight{};

        auto lightsView = scenes.View<LANE::Components::Light,LANE::Components::Transform>(scenes.GetCurrentScene());

        for (auto lightE : lightsView)
        {
            auto& light = scenes.GetComponent<LANE::Components::Light>(scenes.GetCurrentScene(),lightE);
            auto& transform = scenes.GetComponent<LANE::Components::Transform>(scenes.GetCurrentScene(),lightE);

            gpuLight.color = light.color;
            gpuLight.intensity = light.intensity;
            gpuLight.radius = light.radius;
            gpuLight.position = transform.position;

            break;
        }

        glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);

        glBufferSubData(
            GL_UNIFORM_BUFFER,
            0,
            sizeof(GPULight),
            &gpuLight
        );

        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferBase(
            GL_UNIFORM_BUFFER,
            0,
            lightUBO
        );


        for (auto object : objects)
        {
            auto asset =
                assets.GetAsset<ShaderAsset>(69);

            if (!asset)
                continue;

            glUseProgram(asset->shaderProgram);

            auto& transform =
                scenes.GetComponent<Components::Transform>(
                    scenes.GetCurrentScene(),
                    object
                );

            /*
             * MODEL MATRIX
             */

            glm::mat4 model =
                glm::mat4(1.0f);

            model = glm::translate(
                model,
                transform.position
            );

            model = glm::rotate(
                model,
                glm::radians(transform.rotation.x),
                glm::vec3(1.0f, 0.0f, 0.0f)
            );

            model = glm::rotate(
                model,
                glm::radians(transform.rotation.y),
                glm::vec3(0.0f, 1.0f, 0.0f)
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

            /*
             * SEND MATRICES TO SHADER
             */

            glUniformMatrix4fv(
                glGetUniformLocation(
                    asset->shaderProgram,
                    "model"
                ),
                1,
                GL_FALSE,
                glm::value_ptr(model)
            );

            glUniformMatrix4fv(
                glGetUniformLocation(
                    asset->shaderProgram,
                    "view"
                ),
                1,
                GL_FALSE,
                glm::value_ptr(view)
            );

            glUniformMatrix4fv(
                glGetUniformLocation(
                    asset->shaderProgram,
                    "projection"
                ),
                1,
                GL_FALSE,
                glm::value_ptr(projection)
            );

            auto& mesh =
                scenes.GetComponent<Components::Mesh>(
                    scenes.GetCurrentScene(),
                    object
                );

            if (m_vaos[window].find(mesh.uuid) ==
                m_vaos[window].end())
            {
                m_vaos[window][mesh.uuid].Create(
                    mesh.vbo,
                    mesh.ebo
                );
            }

            VAO& vao =
                m_vaos[window][mesh.uuid];

            vao.Load();

            glDrawElements(
                GL_TRIANGLES,
                mesh.indiceCount,
                GL_UNSIGNED_INT,
                0
            );

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
        ImGui_ImplOpenGL3_Init("#version 460");

        m_contexts[window] = context;
    }
}