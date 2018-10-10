#version 330 core
out vec4 FragColor;

in vec4 clipSpace;
in vec3 FragPos;
in vec3 Normal;
in vec2 TextureCoords;

uniform float time;
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform sampler2D reflection_texture;
uniform sampler2D refraction_texture;
uniform sampler2D dudv_texture;

const float dist_factor = 0.03;
void main()
{

  vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5f;
  vec2 refractTexCoords = vec2(ndc.x, ndc.y);
  vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

  vec2 texCoordDist1 = vec2(TextureCoords.x + time, TextureCoords.y);
  vec2 texCoordDist2 = vec2(-TextureCoords.x + time, TextureCoords.y + time);

  vec2 distorsion1 = ((texture(dudv_texture, texCoordDist1).rg * 2.0) - 1.0) * dist_factor;
  vec2 distorsion2 = ((texture(dudv_texture, texCoordDist2).rg * 2.0) - 1.0) * dist_factor;
  vec2 totalDistorsion = distorsion1 + distorsion2;


  reflectTexCoords += totalDistorsion;
  reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
  reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

  refractTexCoords += totalDistorsion;
  refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

  vec4 reflectedColor = texture(reflection_texture, reflectTexCoords);
  vec4 refractedColor = texture(refraction_texture, refractTexCoords);

  FragColor = mix(reflectedColor, refractedColor, 0.5);
  FragColor = mix(FragColor, vec4(0, 0.3, 0.5, 1.0), 0.2);
}
