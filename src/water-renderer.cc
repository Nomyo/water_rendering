#include <water-renderer.hh>
#include <GLFW/glfw3.h>

WaterRenderer::WaterRenderer(Shader shader, glm::mat4 projection_mat,
                             glm::mat4 view_mat, const Light& light,
                             const glm::vec3 cameraPosition)
  : shader_(shader)
{
  shader_.use();
  shader_.setMat4("projection", projection_mat);
  shader_.setMat4("view", view_mat);
  shader_.setVec3("lightPos", light.get_position());
  shader_.setVec3("lightColor", light.get_color());
  shader_.setVec3("cameraPosition", cameraPosition);

  float time = glfwGetTime() * 0.03;

  time = fmod(time, 1.0f);
  shader_.setFloat("time", time);
  shader_.setInt("reflection_texture", 0);
  shader_.setInt("refraction_texture", 1);
  shader_.setInt("dudv_texture", 2);
  shader_.setInt("normal_map", 3);
}

void WaterRenderer::render(Water& w)
{
  prepare_instance(w);
  w.draw(shader_);
}

void WaterRenderer::prepare_instance(const Water& /* w */)
{
  glm::mat4 model;
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(0.0f),
                      glm::vec3(1.0f, 0.0f, 0.0f));
  shader_.setMat4("model", model);
}
