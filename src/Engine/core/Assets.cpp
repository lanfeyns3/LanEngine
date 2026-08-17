#include "Assets.h"

#include <fstream>

namespace LANE
{
    void ShaderAsset::Load(nlohmann::json f)
    {
        // TODO: make it so it compiles in runtime
        std::string vFilePath = f["vertexShaderFile"];
        std::string fFilePath = f["fragmentShaderFile"];

        auto readFile = [](const char* filename) -> std::vector<char>
        {
            std::ifstream file(filename, std::ios::ate | std::ios::binary);

            if (!file)
                throw std::runtime_error("failed to open file");

            size_t size = file.tellg();

            std::vector<char> buffer(size);

            file.seekg(0);
            file.read(buffer.data(), size);

            return buffer;
        };

        auto createShaderModule =
            [&](const char* path) -> VkShaderModule
        {
            auto code = readFile(path);
        
            VkShaderModuleCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            info.codeSize = code.size();
            info.pCode = reinterpret_cast<const uint32_t*>(code.data());
        
            VkShaderModule module;
        
            if (vkCreateShaderModule(
                    device,
                    &info,
                    nullptr,
                    &module) != VK_SUCCESS)
            {
                throw std::runtime_error("shader creation failed");
            }
        
            return module;
        };

        vertex = createShaderModule(vFilePath.c_str());
        fragment = createShaderModule(fFilePath.c_str());
    }

} // namespace LANE
