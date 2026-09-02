#pragma once

#include <glad/glad.h>
#include "VBO.h"
#include "EBO.h"

namespace LANE
{
    class VAO
    {
    public:
        void Create(VBO vbo,EBO ebo);
        void Free();
        
        void Load();
        void Unload();
    private:
        GLuint vao;
    };
} // namespace LANE