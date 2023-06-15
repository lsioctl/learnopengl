#version 330 core

layout (location = 0) in vec3 a_pos;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
  // we read the multiplication from right to left
  gl_Position = projection_matrix * view_matrix * model_matrix * vec4(a_pos, 1.0);
};