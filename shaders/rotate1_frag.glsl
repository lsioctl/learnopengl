#version 330 core

out vec4 frag_color;

in vec2 text_coord;

uniform sampler2D our_texture;
uniform sampler2D our_texture2;

void main()
{
  // mix lineary interpolate the two texture according to the third parameter
  // for 0,2 mix will return 80% of the first input color and 20% of the second
  frag_color = mix(texture(our_texture, text_coord), texture(our_texture2, text_coord), 0.2);
};