#include <water-renderer.hh>
#include <GLFW/glfw3.h>

WaterRenderer::WaterRenderer(Shader shader, glm::mat4 projection_mat,
			     glm::mat4 view_mat, const Light& light)
  : shader_(shader)
{
  shader_.use();
  shader_.setMat4("projection", projection_mat);
  shader_.setMat4("view", view_mat);
  shader_.setVec3("lightPos", light.get_position());
  shader_.setVec3("lightColor", light.get_color());
  shader_.setFloat("time", glfwGetTime());
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