#include <light.hh>
#include <flat_vertices.hh>

Light::Light(glm::vec3 position, glm::vec3 color)
  : position_(position)
  , color_(color)
{
  init();
}

Light::~Light()
{
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
}

void Light::init()
{
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindVertexArray(VAO_);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void Light::draw(Shader /* shader */)
{
  glBindVertexArray(VAO_);
  // Our light is indeed a simple cube.
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

glm::vec3 Light::get_position() const
{
  return position_;
}

glm::vec3 Light::get_color() const
{
  return color_;
}

void Light::set_position(glm::vec3 position)
{
  position_ = position;
}


void Light::set_color(glm::vec3 color)
{
  color_ = color;
}
