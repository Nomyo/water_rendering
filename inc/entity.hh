#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <model.hh>

class Entity
{
public:
  Entity(Model* model, glm::vec3 position, glm::vec3 rotate, float scale);
  virtual ~Entity();

  void increase_position(glm::vec3 p);
  void decrease_position(glm::vec3 p);
  void increase_rotate(glm::vec3 r);
  void decrease_rotate(glm::vec3 r);

  void set_model(Model* model);
  void set_position(glm::vec3 position);
  void set_direction(glm::vec3 direction);
  void set_scale(float scale);

  Model *get_model() const;
  glm::vec3 get_position() const;
  float get_scale() const;
  glm::vec3 get_rotation_axis() const;
  glm::vec3 get_direction() const;

protected:
  glm::vec3 direction_;

private:
  Model *model_;
  glm::vec3 position_;
  glm::vec3 rotate_;
  float scale_;
};
