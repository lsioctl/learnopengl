#pragma once

#include <vector>

#include "Texture.hpp"
#include "ShaderProgram.hpp"

// this will lead later to a more generic Mesh class, but I need
// first to understand what is needed for a Mesh
class CubeWoodSmileMesh
{
private:
    std::size_t Nvertices_;
    std::vector<Texture> texture_list_;
    GLuint VAO_id_;
public:
    CubeWoodSmileMesh();
    void draw(ShaderProgram& shader_program);
};
