#include "VAO.h"

namespace LANE
{
    void VAO::Create(VBO vbo)
    {
        glGenVertexArrays(1,&vao);
        vbo.Unload();
        glBindVertexArray(vao);
        vbo.Load();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);
    }

    void VAO::Free()
    {
        Unload();
        glDeleteVertexArrays(1, &vao);
    }

    void VAO::Load()
    {
        glBindVertexArray(vao);
    }
    
    void VAO::Unload()
    {
        glBindVertexArray(0);
    }
}