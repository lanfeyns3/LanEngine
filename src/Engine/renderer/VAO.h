#pragma once

#include <glad/glad.h>
#include "VBO.h"

namespace LANE
{
    class VAO
    {
    public:
        void Create(VBO vbo);
        void Free();
        
        void Load();
        void Unload();
    private:
        GLuint vao;
    };
} // namespace LANE