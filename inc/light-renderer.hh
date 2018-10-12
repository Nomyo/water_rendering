#pragma once

#include <camera.hh>
#include <mesh.hh>
#include <world.hh>
#include <light.hh>

class LightRenderer
{
public:
  LightRenderer(Shader shader, glm::mat4 projection_mat,
                glm::mat4 view_mat);
  ~LightRenderer() = default;

  void render(Light& l);
  void prepare_instance(const Light& l);

private:
  Shader shader_;
};
