#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_norm;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat3 normal_matrix;

out vec3 normal;
out vec3 frag_pos;

void main()
{
  // we read the multiplication from right to left
  gl_Position = projection_matrix * view_matrix * model_matrix * vec4(a_pos, 1.0);
  normal = normal_matrix * a_norm;
  frag_pos = vec3(model_matrix * vec4(a_pos, 1.0));
};