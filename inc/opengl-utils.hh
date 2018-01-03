#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <string>

#include <glob_conf.hh>
#include <world.hh>

/**
 * Whenever the window size changes this funtion is called
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

/**
 * Generate a texture and return the texture_id
 */
unsigned int gen_texture(const std::string& path);

/**
 * Initialisation of the screen
 */
GLFWwindow *window_init();
