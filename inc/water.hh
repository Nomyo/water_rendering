#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <shader_m.hh>
#include <mesh.hh>

#include <iostream>

class Water
{
public:
  Water(glm::vec3 position, unsigned int width, unsigned int height);
  Water(glm::vec3 position, unsigned int width, unsigned int height,
	const std::string& texture_file);
  ~Water() = default;

  void draw(Shader shader);
  void create_mesh(const std::string& texture_file);
  Mesh get_mesh() const;

private:
  Mesh mesh_;
  glm::vec3 position_;
  unsigned int width_;
  unsigned int height_;
};
