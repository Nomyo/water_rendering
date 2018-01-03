#include <main.hh>

int start_opengl()
{
  GLFWwindow *window = window_init();
  if (!window)
    return 1;
  auto& global_conf = GlobalConf::get_instance();
  auto camera = global_conf.get_camera();
  camera->set_camera(glm::vec3(30.0f, 15.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		     90.0f, 0.0f);

  // Create our shader
  Shader world_shader("shaders/world.vs", "shaders/world.fs");

  auto world = World("map/heightmap_01.png");

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  float updateFrame = 0.0f;
  while (!glfwWindowShouldClose(window))
  {
    // per-frame time logic
    // --------------------
    float currentFrame = glfwGetTime();
    GlobalConf::deltaTime = currentFrame - GlobalConf::lastFrame;
    GlobalConf::lastFrame = currentFrame;
    updateFrame += GlobalConf::deltaTime;

    // input
    // -----
    global_conf.process_input(window);

    // render
    // ------
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(
      glm::radians(camera->get_zoom()),
      (float)GlobalConf::SCR_WIDTH / (float)GlobalConf::SCR_HEIGHT,
      0.1f, 500.0f);

    glm::mat4 view = camera->get_view_matrix();

    // render object
    // -------------
    WorldRenderer wr(world_shader, projection, view);
    wr.render(world);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

int main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  return start_opengl();
}
