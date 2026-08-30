#pragma once

#include <stdint.h>
#include <string>

#include <glm/glm.hpp>

namespace LANE
{
    namespace Components
    {
        struct Info
        {
            uint64_t uuid;
            std::string name;
        };

        struct Renderer // TODO: better name
        {

        };

        struct Transform
        {
            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scale;
        };
    } // namespace Components
    
} // namespace LANE
