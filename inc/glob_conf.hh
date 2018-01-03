#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <camera.hh>

class GlobalConf
{

public:
    static GlobalConf& get_instance();
    static void process_input(GLFWwindow *window);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset,
				double yoffset);
    void init(Camera *camera);
    Camera *get_camera();

private:
    GlobalConf() { };
    GlobalConf(GlobalConf const&) = delete;
    void operator=(const GlobalConf&) = delete;

    Camera *camera_;

public:
    static constexpr unsigned int SCR_WIDTH = 1300;
    static constexpr unsigned int SCR_HEIGHT = 950;

    static float lastX_;
    static float lastY_;
    static float deltaTime;
    static float lastFrame;
    static bool first_mouse_;
};
