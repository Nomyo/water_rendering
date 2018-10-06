#version 330 core
out vec4 FragColor;

in vec4 clipSpace;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform sampler2D reflection_texture;
uniform sampler2D refraction_texture;

void main()
{

  vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5f;
  vec2 refractTexCoords = vec2(ndc.x, ndc.y);
  vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

  vec4 reflectedColor = texture(reflection_texture, reflectTexCoords);
  vec4 refractedColor = texture(refraction_texture, refractTexCoords);

  FragColor = mix(reflectedColor, refractedColor, 0.5);
}
