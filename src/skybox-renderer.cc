#include <skybox-renderer.hh>

SkyboxRenderer::SkyboxRenderer(Shader shader, glm::mat4 projection_mat,
			     glm::mat4 view_mat)
  : shader_(shader)
{
  shader_.use();
  shader_.setMat4("projection", projection_mat);
  shader_.setMat4("view", view_mat);
}

void SkyboxRenderer::render(Skybox& s)
{
  // change depth function so depth test passes when values are equal to depth
  // buffer's content
  s.draw();
}
