#pragma once

#include <camera.hh>
#include <mesh.hh>
#include <water.hh>
#include <light.hh>

class WaterRenderer
{
public:
  WaterRenderer(Shader shader, glm::mat4 projection_mat,
                glm::mat4 view_mat, const Light& light,
                const glm::vec3 cameraPosition);
  ~WaterRenderer() = default;

  void render(Water& w);
  void prepare_instance(const Water& w);

private:
  Shader shader_;
};
