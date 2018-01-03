#include <light-renderer.hh>

LightRenderer::LightRenderer(Shader shader, glm::mat4 projection_mat,
			     glm::mat4 view_mat)
  : shader_(shader)
{
  shader_.use();
  shader_.setMat4("projection", projection_mat);
  shader_.setMat4("view", view_mat);
}

void LightRenderer::render(Light& l)
{
  prepare_instance(l);
  l.draw(shader_);
}

void LightRenderer::prepare_instance(const Light& l)
{
  glm::mat4 model;
  model = glm::translate(model, l.get_position());
  model = glm::scale(model, glm::vec3(3.0f));
  shader_.setMat4("model", model);
}
