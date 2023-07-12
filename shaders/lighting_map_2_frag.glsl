#version 330 core

in vec3 normal;
// this will be interpolated from the 3 trianges position vectors
// to create the per fragment position
in vec3 frag_pos;
in vec2 text_coord;

out vec4 frag_color;

uniform vec3 camera_pos;

struct Light {
  vec3 position;
  // usually set to a low intesity
  vec3 ambient;
  // usually exact color we want a light to have
  vec3 diffuse;
  // usually set at full intensity vec3(1.0)
  vec3 specular;
};

uniform Light light;

struct Material {
  // ambient color is now equal to the diffuse
  // color now that we control ambient color with the light
  // so we merge ambient and diffuse
  // sampler2D is an opaque type (we can't instanciate it,
  // only use them as uniform)
  sampler2D diffuse;
  // color under specular lighting
  sampler2D specular;
  // scattering/radius of the specular light
  float shininess;
};

uniform Material material;


void main()
{
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, text_coord));

  // we need to normalize before the dot product
  // as we only care of the direction
  vec3 norm = normalize(normal);
  vec3 light_dir = normalize(light.position - frag_pos);
  // if the angle between the vectors is more than 90 degrees,
  // the dot product becomes negative, so avoid this with max
  float diff = max(dot(norm, light_dir), 0.);
  vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, text_coord)));

  vec3 view_dir = normalize(camera_pos - frag_pos);
  // the reflect function expect the first vector
  // to point from the light source toward the fragment position
  // ours is reversed, so we negate it
  vec3 reflection_dir = reflect(-light_dir, norm);
  float spec = pow(max(dot(view_dir, reflection_dir), 0.), material.shininess);
  vec3 specular = light.specular * (spec * vec3(texture(material.specular, text_coord)));

  frag_color = vec4((ambient + diffuse + specular), 1.0);
};