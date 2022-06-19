#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_text_coord;

out vec2 text_coord;

uniform mat4 transform;

void main()
{
  gl_Position = transform * vec4(a_pos, 1.0);
  text_coord = a_text_coord;
};