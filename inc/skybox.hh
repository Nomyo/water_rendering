#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <shader_m.hh>

#include <vector>
#include <iostream>

class Skybox
{
public:
  Skybox(const std::vector<std::string>& faces);
  ~Skybox() = default;

  void init(Shader& shader);
  void draw();

private:
  std::vector<std::string> faces_;
  unsigned texture_id_;
  unsigned int VBO_;
  unsigned int VAO_;
};
