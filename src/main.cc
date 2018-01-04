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
  Shader skybox_shader("shaders/skybox.vs", "shaders/skybox.fs");

  std::vector<std::string> faces{"textures/sky-right.jpg", "textures/sky-left.jpg",
      "textures/sky-top.jpg", "textures/sky-bottom.jpg", "textures/sky-back.jpg",
      "textures/sky-front.jpg" };

  auto world = World("map/heightmap_03.png");
  auto water = Water(glm::vec3{12.0f, 12.5f, 12.0f}, 24.5, 24.5, "water.jpg");
  auto light = Light(glm::vec3{world.get_width(), 55.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f});
  auto skybox = Skybox(faces);
  skybox.init(skybox_shader);

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

    // For now do not display the light, use the sun of the skybox
    // LightRenderer lr(light_shader, projection, view);
    // lr.render(light);

    // remove translation from the view matrix
    glDepthFunc(GL_LEQUAL);
    view = glm::mat4(glm::mat3(camera->get_view_matrix()));
    SkyboxRenderer sr(skybox_shader, projection, view);
    sr.render(skybox);
    glDepthFunc(GL_LESS);

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
