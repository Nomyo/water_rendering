#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 pix_pos;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

void main()
{
  vec4 totalColour;
  if (pix_pos.y < 10.0f)
    totalColour = texture(texture_diffuse2, TexCoords);
  else if (pix_pos.y < 12.0f && pix_pos.y > 11.0f) {
    vec4 t2 = texture(texture_diffuse2, TexCoords);
    vec4 t1 = texture(texture_diffuse1, TexCoords);
    totalColour = mix(t2, t1, 0.5);
  }
  else if (pix_pos.y <= 11.0f && pix_pos.y > 10.0f) {
    vec4 t2 = texture(texture_diffuse2, TexCoords);
    vec4 t1 = texture(texture_diffuse1, TexCoords);
    totalColour = mix(t1, t2, 0.75);
  }
  else
    totalColour = texture(texture_diffuse1, TexCoords);

  // ambient
  float ambientStrength = 0.2;
  vec3 ambient = ambientStrength * lightColor;

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  vec3 resultColor = (ambient + diffuse) * totalColour.xyz;
  FragColor = vec4(resultColor.x, resultColor.y, resultColor.z, 1.0f);
}
