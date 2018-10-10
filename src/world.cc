#include <world.hh>
#include <opengl-utils.hh>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdio.h>

namespace
{
  Uint32 get_color_aux(const SDL_Surface* surface, int x, int y)
  {
    int bpp = surface->format->BytesPerPixel;

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp)
    {
    case 1:
      return *p;
    case 2:
      return *(Uint16 *)p;
    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
        return p[0] << 16 | p[1] << 8 | p[2];
      else
        return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
      return *(Uint32 *)p;
    default:
      return 0;
    }
  }

  float get_grey_value(const SDL_Surface *surface, int x, int y)
  {
    Uint8 r;
    Uint8 g;
    Uint8 b;

    SDL_GetRGB(get_color_aux(surface, x, y), surface->format, &r, &g, &b);
    return (r + g + b) / 3.0f;
  }


  std::vector<Vertex> read_height_map(const std::string& file,
                                      unsigned int *height,
                                      unsigned int *width)
  {
    std::vector<Vertex> vertices;
    SDL_Surface *image = IMG_Load(file.c_str());
    if (!image)
    {
      std::cerr << "Couln't load height map" << std::endl;
      return vertices;
    }

    *height = image->h;
    *width = image->w;

    for (int z = 0; z < image->h; ++z)
    {
      for (int x = 0; x < image->w; ++x)
      {
        auto color = get_grey_value(image, x, z) / 15.0f;
        vertices.emplace_back(Vertex{glm::vec3(x, color, z),
              glm::vec3(0.0f, 2.0f, 0.0f), glm::vec2(x, z),
              glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f}});
      }
    }

    SDL_FreeSurface(image);
    return vertices;
  }

  std::vector<Vertex> default_map(unsigned int width, unsigned int height)
  {
    std::vector<Vertex> vertices;
    for (unsigned int z = 0; z < height; ++z)
    {
      for (unsigned int x = 0; x < width; ++x)
      {
        vertices.emplace_back(Vertex{glm::vec3(x, 0.0f, z),
              glm::vec3(0.0f, 2.0f, 0.0f), glm::vec2(x, z),
              glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f}});
      }
    }
    return vertices;
  }
} // namespace

World::World(unsigned int width, unsigned int height)
  : width_(width)
  , height_(height)
{
  std::vector<Vertex> vertices = default_map(width_, height_);
  create_mesh(vertices);
}

World::World(const std::string& file)
{
  std::vector<Vertex> vertices = read_height_map(file, &height_, &width_);
  create_mesh(vertices);
}

World::World(Mesh mesh)
  : mesh_(mesh)
{ }

World::~World()
{
  mesh_.free_resources();
}

void World::create_mesh(std::vector<Vertex>& vertices)
{
  std::vector<unsigned int> indices;

  // set indices
  for (unsigned int z = 0; z < height_ - 1; ++z)
  {
    for (unsigned int x = 0; x < width_ - 1; ++x)
    {
      int start = x + z * width_;
      indices.push_back(start);
      indices.push_back(start + 1);
      indices.push_back(start + width_);
      indices.push_back(start + 1);
      indices.push_back(start + 1 + width_);
      indices.push_back(start + width_);
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
  texture.id = gen_texture("grass2.jpg");
  texture.type = "texture_diffuse";
  texture.path = "textures/grass2.jpg";
  textures.push_back(texture);

  Texture texture2;
  texture2.id = gen_texture("sand.png");
  texture2.type = "texture_diffuse";
  texture2.path = "textures/sand.png";
  textures.push_back(texture2);

  mesh_ = Mesh(vertices, indices, textures);
}

void World::draw(Shader shader)
{
  mesh_.draw(shader);
}

unsigned int World::get_width() const
{
  return width_;
}

unsigned int World::get_height() const
{
  return height_;
}

Mesh World::get_mesh() const
{
  return mesh_;
}
