#version 330 core

out vec4 frag_color;

in vec2 text_coord;

uniform sampler2D our_texture;
uniform vec4 our_color;

void main()
{
  frag_color = texture(our_texture, text_coord) * our_color; ;
};