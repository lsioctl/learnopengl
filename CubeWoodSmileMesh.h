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
    // static to avoid creating duplicate shaders and textures
    inline static bool instance_exists_;
    GLuint VAO_id_;
public:
    CubeWoodSmileMesh();
    void draw(ShaderProgram& shader_program);
};
