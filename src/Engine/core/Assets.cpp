#include "Assets.h"

#include <fstream>

namespace LANE
{
    void ShaderAsset::Load(nlohmann::json f)
    {
        std::string vGlslSource = f["vertexShaderCode"];
        std::string fGlslSource = f["fragmentShaderCode"];

        auto compileGlslToSpv = [](const std::string& source, EShLanguage stage) -> std::vector<uint32_t>
        {
            glslang::InitializeProcess();

            glslang::TShader shader(stage);
            const char* shaderStrings = source.c_str();
            shader.setStrings(&shaderStrings, 1);

            shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientVulkan, 110);
            shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_2);
            shader.setEnvTarget(glslang::EShTargetLanguage::EShTargetSpv, glslang::EShTargetSpv_1_3);

            TBuiltInResource resources = *GetDefaultResources();

            if (!shader.parse(
                &resources,
                100,
                false,
                EShMsgDefault))
            {
                std::string errorLog = shader.getInfoLog();
            
                glslang::FinalizeProcess();
            
                throw std::runtime_error(
                    "GLSL Parsing Failed:\n" + errorLog
                );
            }

            glslang::TProgram program;
            program.addShader(&shader);

            if (!program.link(EShMsgDefault)) {
                std::string errorLog = program.getInfoLog();
                glslang::FinalizeProcess();
                throw std::runtime_error("GLSL Linking Failed:\n" + errorLog);
            }

            std::vector<uint32_t> spirv;
            glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);

            glslang::FinalizeProcess();
            return spirv;
        };

        auto createShaderModuleFromSpv = [&](const std::vector<uint32_t>& spirv) -> VkShaderModule
        {
            VkShaderModuleCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            info.codeSize = spirv.size() * sizeof(uint32_t);
            info.pCode = spirv.data();

            VkShaderModule module;
            if (vkCreateShaderModule(device, &info, nullptr, &module) != VK_SUCCESS)
            {
                throw std::runtime_error("shader module creation failed");
            }

            return module;
        };

        std::vector<uint32_t> vertSpv = compileGlslToSpv(vGlslSource, EShLangVertex);
        vertex = createShaderModuleFromSpv(vertSpv);
        
        std::vector<uint32_t> fragSpv = compileGlslToSpv(fGlslSource, EShLangFragment);
        fragment = createShaderModuleFromSpv(fragSpv);
    }

} // namespace LANE
