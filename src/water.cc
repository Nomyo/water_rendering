#include <water.hh>
#include <opengl-utils.hh>
#include <stdint.h>
#include <stdio.h>


Water::Water(glm::vec3 position, unsigned int width, unsigned int height)
  : position_(position)
  , width_(width)
  , height_(height)
{

}

Water::Water(glm::vec3 position, unsigned int width, unsigned int height,
	     const std::string& texture_file)
  : position_(position)
  , width_(width)
  , height_(height)
{
  create_mesh(texture_file);
}

void Water::create_mesh(const std::string& texture_file)
{
  std::vector<unsigned int> indices;
  std::vector<Vertex> vertices;

  for (unsigned int z = 0; z < height_; ++z)
  {
    for (unsigned int x = 0; x < width_; ++x)
    {
      vertices.emplace_back(Vertex{glm::vec3(x + position_.x ,
					     0.0f + position_.y,
					     z + position_.z),
	    glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(x, z),
	    glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f}});
    }
  }

  // set indices
  for (unsigned int z = 0; z < height_ - 1; ++z)
  {
    for (unsigned int x = 0; x < width_ - 1; ++x)
    {
      int start = x + z * height_;
      indices.push_back(start);
      indices.push_back(start + 1);
      indices.push_back(start + height_);
      indices.push_back(start + 1);
      indices.push_back(start + 1 + height_);
      indices.push_back(start + height_);
    }
  }

  // set normals
  for (unsigned int z = 0; z < height_ - 1; ++z)
  {
    for (unsigned int x = 0; x < width_ - 1; ++x)
    {
      if (x != 0 && x != width_ - 1 && z != 0 && z != height_ - 1)
      {
  	float height_l = vertices[z * width_ + x - 1].Position.y;
  	float height_r = vertices[z * width_ + x + 1].Position.y;
  	float height_d = vertices[(z - 1) * width_ + x].Position.y;
  	float height_u = vertices[(z + 1) * width_ + x].Position.y;
  	glm::vec3 normal{height_l - height_r, 2.0f, height_d - height_u};
  	normal = glm::normalize(normal);
  	vertices[z * width_ + x].Normal = normal;
      }
    }
  }

  std::vector<Texture> textures;
  Texture texture;
  texture.id = gen_texture(texture_file);
  texture.type = "texture_diffuse";
  texture.path = "textures/" + texture_file; //"textures/water.jpg";
  textures.push_back(texture);

  mesh_ = Mesh(vertices, indices, textures);
}

void Water::draw(Shader shader)
{
  mesh_.draw(shader);
}

Mesh Water::get_mesh() const
{
  return mesh_;
}
