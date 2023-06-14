#version 330 core

layout (location = 0) in vec3 a_pos;

void main()
{
  // we read the multiplication from right to left
  gl_Position = projection_matrix * view_matrix * model_matrix * vec4(a_pos, 1.0);
};