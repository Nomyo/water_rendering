#pragma once

#include <glad/glad.h>

#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader_m.hh>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

struct Vertex
{
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
  glm::vec3 Tangent;
  glm::vec3 Bitangent;
};

struct Texture
{
  unsigned int id;
  std::string type;
  aiString path;
};

class Mesh
{
public:
  Mesh() = default;
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<Texture> textures);

  void free_resources();
  void draw(Shader shader);
  std::vector<Vertex> get_vertices() const;

private:
  void setup_mesh();

private:
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  std::vector<Texture> textures_;

  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
};
