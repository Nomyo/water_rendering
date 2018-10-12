#version 330 core
out vec4 FragColor;

in vec4 clipSpace;
in vec3 FragPos;
in vec3 Normal;
in vec2 TextureCoords;
in vec3 toCamera;
in vec3 fromLight;

uniform float time;
uniform vec3 lightColor;

uniform sampler2D reflection_texture;
uniform sampler2D refraction_texture;
uniform sampler2D dudv_texture;
uniform sampler2D normal_map;

const float dist_factor = 0.03;

void main()
{

  vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5f;
  vec2 refractTexCoords = vec2(ndc.x, ndc.y);
  vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

  // Distorsion
  vec2 texCoordDist = texture(dudv_texture, vec2(TextureCoords.x + time, TextureCoords.y)).rg * 0.1;
  texCoordDist = TextureCoords + vec2(texCoordDist.x, texCoordDist.y + time);
  vec2 totalDistorsion = (texture(dudv_texture, texCoordDist).rg * 2.0 - 1.0) * dist_factor;

  reflectTexCoords += totalDistorsion;
  reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
  reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

  refractTexCoords += totalDistorsion;
  refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

  vec4 reflectedColor = texture(reflection_texture, reflectTexCoords);
  vec4 refractedColor = texture(refraction_texture, refractTexCoords);

  // Normal mapping
  vec3 toCameraNormalized = normalize(toCamera);
  float refractorVector = dot(toCameraNormalized, vec3(0.0, 1.0, 0.0));
  refractorVector = pow(refractorVector, 0.5);

  vec4 normalMapColor = texture(normal_map, texCoordDist);
  vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0 , normalMapColor.b, normalMapColor.g * 2.0 - 1.0);
  normal = normalize(normal);

  vec3 reflectedLight = reflect(normalize(fromLight), normal);
  float specular = max(dot(reflectedLight, toCamera), 0.0);
  vec3 specularHighlights = lightColor * specular * 0.015;

  // Final color
  FragColor = mix(reflectedColor, refractedColor, refractorVector);
  FragColor = mix(FragColor, vec4(0, 0.3, 0.6, 1.0), 0.2) + vec4(specularHighlights, 0.0);
}
