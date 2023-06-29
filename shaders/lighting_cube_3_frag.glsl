#version 330 core

in vec3 normal;
// this will be interpolated from the 3 trianges position vectors
// to create the per fragment position
in vec3 frag_pos;

out vec4 frag_color;

uniform vec3 color;
uniform vec3 light_color;
uniform vec3 light_pos;

void main()
{
  float ambient_strength = 0.5;
  vec3 ambient = ambient_strength * light_color;
  // we need to normalize before the dot product
  // as we only car of the direction
  vec3 norm = normalize(normal);
  vec3 light_dir = normalize(light_pos - frag_pos);
  // if the angle between the vectors is more than 90 degrees,
  // the dot product becomes negative, so avoid this with max
  float diff = max(dot(norm, light_dir), 0.);
  vec3 diffuse = diff * light_color;

  frag_color = vec4((ambient + diffuse) * color, 1.0);
};