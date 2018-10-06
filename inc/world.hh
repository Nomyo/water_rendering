#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <mesh.hh>
#include <shader_m.hh>

class World
{
public:
  World(unsigned int width, unsigned int height);
  World(const std::string& file);
  World(Mesh mesh);

  ~World();

  void draw(Shader shader);
  void create_mesh(std::vector<Vertex>& vertices);
  Mesh get_mesh() const;
  unsigned int get_width() const;
  unsigned int get_height() const;

private:
  Mesh mesh_;
  unsigned int width_;
  unsigned int height_;
};
