#pragma once

#include <camera.hh>
#include <mesh.hh>
#include <world.hh>
#include <skybox.hh>

class SkyboxRenderer
{
public:
  SkyboxRenderer(Shader shader, glm::mat4 projection_mat,
		glm::mat4 view_mat);
  ~SkyboxRenderer() = default;

  void render(Skybox& l);

private:
  Shader shader_;
};
