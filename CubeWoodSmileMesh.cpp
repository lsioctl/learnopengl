#include <vector>
#include <iostream>

#include "glad/glad.h"

#include "CubeWoodSmileMesh.hpp"


CubeWoodSmileMesh::CubeWoodSmileMesh()
{
    // TODO: singleton ?
    // vertices in normalized device coordinates (visible region of OpenGL)
    // We don't use EBO here, because of the texture coordinates (see world_coo2 result in this case)
    std::vector<float> vertices{
        // positions          // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    Nvertices_ = vertices.size();

    // Vertex Array Object
    // It will allow to store VBO and Vertex Attribute that
    // we will consequently declare
    // in Core OpenGL, VAO are mandatory
    std::cout << VAO_id_ << std::endl;
    glGenVertexArrays(1, &VAO_id_);
    std::cout << VAO_id_ << std::endl;

    // once we bind it, all subsequent VBO and attributes
    // will be store on the VAO referenced by this id
    glBindVertexArray(VAO_id_);

    // Vertex Buffer Object
    // will set up a memory on the graphic card
    // VBO will store its buffer id
    unsigned int VBO;
    glGenBuffers(1, &VBO);  
    // sGL_ARRAY_BUFFER is the type for a vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    // copy the vertex data into buffer memory for the currently
    // bound buffer (VBO)
    // the 4th parameters tells how we want the graphic card to manage the data
    // GL_STATIC_DRAW means the data is set up once and could be used many times
    // (so a memory region where read is fast could be good)
    // another example is GL_DYNAMIC_DRAW, which means thata data can change a lot
    // so it should be stored in a memory region where write (and read for usage) is fast
    // TODO: size dangerous if refactored: a wrong size will prevent any display
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Now we have to have to tell OpenGL how to interpret the raw data
    // vertices
    glVertexAttribPointer(
        0, // location of the vertex attribute, will be used in vertex shader with layout (location = 0)
        3, // vertex attribue is a vec3, 3 elements
        GL_FLOAT, // element of the vec3 is float
        GL_FALSE, // don't normalize data
        5 * sizeof(float), // size of a stride is 8 float elements
        (void*)0 // offset is 0
    );

    // vertex attributes are disabled by default, so enable the one we created
    glEnableVertexAttribArray(0);

    // textures
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    texture_list_.reserve(2);
    // TODO: harcoded relative path
    texture_list_.push_back(Texture{"./textures/container.jpg", GL_RGB});
    texture_list_.push_back(Texture{"./textures/awesomeface.png", GL_RGBA});

    // use texture units to bind multiple textures at once
    glActiveTexture(GL_TEXTURE0);
    texture_list_[0].bind();
    glActiveTexture(GL_TEXTURE1);
    texture_list_[1].bind();

    // unbind VAO by binding to the default one
    glBindVertexArray(0);
}


void CubeWoodSmileMesh::draw(ShaderProgram& shader_program) {
    glBindVertexArray(VAO_id_);

    // activate the shader (TODO: most probably already used)
    shader_program.use();

    // Now shader is in use, we can set the uniforms for the texture units
    shader_program.setInt("our_texture", 0);
    shader_program.setInt("our_texture2", 1);

    // render the cube
    glDrawArrays(
        GL_TRIANGLES,  // we want to draw triangles
        0,
        Nvertices_  // we want this number of vertices in total
    );
}