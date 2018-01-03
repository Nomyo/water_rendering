#pragma once

#include <camera.hh>
#include <mesh.hh>
#include <world.hh>
#include <light.hh>

class WorldRenderer
{
public:
  WorldRenderer(Shader shader, glm::mat4 projection_mat,
		glm::mat4 view_mat, const Light& light);
  ~WorldRenderer() = default;

  void render(World& w);
  void prepare_instance(const World& w);

private:
  Shader shader_;
};
