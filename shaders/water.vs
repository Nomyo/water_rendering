#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec4 clipSpace;
out vec3 FragPos;
out vec3 Normal;
out vec2 TextureCoords;
out vec3 toCamera;
out vec3 fromLight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPosition;
uniform vec3 lightPos;

const float tiling = 0.20;

void main()
{
  FragPos = vec3(model * vec4(aPos, 1.0));
  vec4 world_position = vec4(FragPos, 1.0);
  gl_Position = projection * view * world_position;
  clipSpace = gl_Position;
  Normal = aNormal;
  TextureCoords = vec2((aTexCoords.x / 2.0) + 0.5, (aTexCoords.y / 2.0) + 0.5) * tiling;
  toCamera = cameraPosition - world_position.xyz;
  fromLight = world_position.xyz - lightPos;
}
