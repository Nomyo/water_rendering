#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
  vec3 vPos = aPos;
  vPos.y += sin(time * 2.0 + (vPos.x * vPos.z * 1.0)) * 0.2;

  FragPos = vec3(model * vec4(vPos, 1.0));
  vec4 world_position = vec4(FragPos, 1.0);
  gl_Position = projection * view * world_position;

  Normal = aNormal;
  TexCoords = aTexCoords;
}