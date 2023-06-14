#version 330 core

out vec4 frag_color;

uniform vec3 color;

void main()
{
  // mix lineary interpolate the two texture according to the third parameter
  // for 0,2 mix will return 80% of the first input color and 20% of the second
  frag_color = (color, 1);
};