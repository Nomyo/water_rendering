#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <shader_m.hh>

#include <iostream>

class Light
{
public:
  Light(glm::vec3 position, glm::vec3 color);
  ~Light();

  void init();
  void draw(Shader shader);
  glm::vec3 get_position() const;
  glm::vec3 get_color() const;
  void set_position(glm::vec3 position);
  void set_color(glm::vec3 color);

private:
  glm::vec3 position_;
  glm::vec3 color_;
  unsigned int VAO_;
  unsigned int VBO_;
};
