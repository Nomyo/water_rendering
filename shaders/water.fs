#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform sampler2D texture_diffuse1;

void main()
{
  FragColor = vec4(0, 0, 1.0f, 1.0f);
}
