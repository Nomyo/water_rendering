#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <mesh.hh>
#include <shader_m.hh>

class World
{
public:
    World(unsigned int width, unsigned int height);
    World(Mesh mesh);

    ~World() = default;

    void draw(Shader shader);
    void create_mesh();
    Mesh get_mesh() const;

private:
    Mesh mesh_;
    unsigned int width_;
    unsigned int height_;
};
