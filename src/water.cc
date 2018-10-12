#include <water.hh>
#include <opengl-utils.hh>
#include <stdint.h>
#include <iostream>

Water::Water(glm::vec3 position, unsigned int width, unsigned int height)
  : position_(position)
  , width_(width)
  , height_(height)
{
  create_mesh();
  create_frame_buffers();
  dudv_texture_ = gen_texture("dudv.jpg");
  normal_map_ = gen_texture("normal_map.png");
}

Water::~Water()
{
  mesh_.free_resources();
  glDeleteFramebuffers(1, &reflectionFrameBuffer);
  glDeleteTextures(1, &reflectionTexture);
  glDeleteRenderbuffers(1, &reflectionDepthBuffer);

  glDeleteFramebuffers(1, &refractionFrameBuffer);
  glDeleteTextures(1, &refractionTexture);
  glDeleteRenderbuffers(1, &refractionDepthBuffer);
}

void Water::create_mesh()
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
  mesh_ = Mesh(vertices, indices, textures);
}

void Water::create_frame_buffers()
{
  // Reflection
  glGenFramebuffers(1, &reflectionFrameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, reflectionFrameBuffer);

  glGenTextures(1, &reflectionTexture);
  glBindTexture(GL_TEXTURE_2D, reflectionTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, reflectionWidth, reflectionHeight, 0,
               GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         reflectionTexture, 0);

  glGenRenderbuffers(1, &reflectionDepthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, reflectionDepthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, reflectionWidth,
                        reflectionHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, reflectionDepthBuffer);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

  glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind

  // Refraction
  glGenFramebuffers(1, &refractionFrameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, refractionFrameBuffer);

  glGenTextures(1, &refractionTexture);
  glBindTexture(GL_TEXTURE_2D, refractionTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, refractionWidth, refractionHeight, 0,
               GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         refractionTexture, 0);

  glGenRenderbuffers(1, &refractionDepthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, refractionDepthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, refractionWidth,
                        refractionHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, refractionDepthBuffer);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

  glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind
}

void Water::bind_reflection_fb()
{
  glBindFramebuffer(GL_FRAMEBUFFER, reflectionFrameBuffer);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Water::bind_refraction_fb()
{
  glBindFramebuffer(GL_FRAMEBUFFER, refractionFrameBuffer);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Water::draw(Shader shader)
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, reflectionTexture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, refractionTexture);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, dudv_texture_);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, normal_map_);

  mesh_.draw(shader);
}

Mesh Water::get_mesh() const
{
  return mesh_;
}

float Water::get_height() const
{
  return position_.y;
}
