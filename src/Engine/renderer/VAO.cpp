#include "VAO.h"

namespace LANE
{
    void VAO::Create(VBO vbo,EBO ebo)
    {
        glGenVertexArrays(1,&vao);
        vbo.Unload();
        ebo.Unload();
        glBindVertexArray(vao);
        vbo.Load();
        ebo.Load();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
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