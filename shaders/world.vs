#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec3 pix_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 plane;

void main()
{
  FragPos = vec3(model * vec4(aPos, 1.0));
  vec4 world_position = vec4(FragPos, 1.0);
  gl_Position = projection * view * world_position;

  gl_ClipDistance[0] = dot(world_position, plane);

  Normal = aNormal;
  TexCoords = aTexCoords;
  pix_pos = aPos;
}
