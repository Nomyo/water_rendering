#include <main.hh>

int start_opengl()
{
  GLFWwindow *window = window_init();
  if (!window)
    return 1;
  auto& global_conf = GlobalConf::get_instance();
  auto camera = global_conf.get_camera();
  camera->set_camera(glm::vec3(30.0f, 25.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		     90.0f, 0.0f);

  // Create our shader
  Shader world_shader("shaders/world.vs", "shaders/world.fs");
  Shader water_shader("shaders/water.vs", "shaders/water.fs");
  Shader light_shader("shaders/light.vs", "shaders/light.fs");

  auto world = World("map/heightmap_03.png");
  auto water = Water(glm::vec3{12.0f, 12.5f, 12.0f}, 24.5, 24.5, "water.jpg");
  auto light = Light(glm::vec3{0.0f, 55.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f});

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  float updateFrame = 0.0f;
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    WorldRenderer wr(world_shader, projection, view, light);
    wr.render(world);

    WaterRenderer wt(water_shader, projection, view, light);
    wt.render(water);

    LightRenderer lr(light_shader, projection, view);
    lr.render(light);

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
