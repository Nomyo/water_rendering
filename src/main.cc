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
  Shader tree_shader("shaders/model.vs", "shaders/tree.fs");
  Shader rock_shader("shaders/model.vs", "shaders/rock.fs");

  std::vector<std::string> faces{"textures/sky-left.jpg", "textures/sky-right.jpg",
      "textures/sky-top.jpg", "textures/sky-bottom.jpg", "textures/sky-front.jpg",
      "textures/sky-back.jpg" };

  // Init environment entities
  auto tree_model = Model("textures/pine.obj", "textures/pine.png", "", false);
  auto rock_model = Model("textures/rock1.obj", "textures/rock1.jpg", "", false);

  auto tree1 = Entity(&tree_model, {18, 11.5, 15}, {0, 1, 0}, 0.8);
  auto tree2 = Entity(&tree_model, {35, 11.5, 14}, {0, 15, 0}, 0.55);
  auto tree3 = Entity(&tree_model, {32, 11.5, 10}, {0, 10, 0}, 0.65);
  auto tree4 = Entity(&tree_model, {12, 11.5, 25}, {0, 1, 0}, 0.75);
  auto tree5 = Entity(&tree_model, {10, 11.5, 10}, {0, 55, 0}, 0.45);
  auto tree6 = Entity(&tree_model, {40, 11.5, 30}, {0, 55, 0}, 0.65);
  auto tree7 = Entity(&tree_model, {35, 11.5, 35}, {0, 55, 0}, 0.75);
  auto tree8 = Entity(&tree_model, {30, 11.5, 40}, {0, 55, 0}, 0.45);
  auto rock = Entity(&rock_model, {20, 12, 35}, {0, 1, 0}, 0.02);
  auto rock2 = Entity(&rock_model, {18, 13, 36}, {0, 10, 0}, 0.01);
  auto rock3 = Entity(&rock_model, {22, 13, 40}, {0, 1, 1}, 0.015);

  auto world = World("map/heightmap_03.png");
  auto water = Water(glm::vec3{12.0f, 12.5f, 12.0f}, 24.5, 24.5);
  auto light = Light(glm::vec3{15.0f / 2, 30.0f, 30.0f},
                     glm::vec3{1.0f, 1.0f, 1.0f});
  auto skybox = Skybox(faces);
  skybox.init(skybox_shader);

  float update_frame = 0.0f;
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  while (!glfwWindowShouldClose(window)) {

    // per-frame time logic
    // --------------------
    float current_frame = glfwGetTime();
    GlobalConf::deltaTime = current_frame - GlobalConf::lastFrame;
    GlobalConf::lastFrame = current_frame;
    update_frame += GlobalConf::deltaTime;

    // input
    // -----
    global_conf.process_input(window);

    auto render_scene = [&](const glm::vec4 clip_plane, bool fb=false)
      {
        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(
                                                glm::radians(camera->get_zoom()),
                                                (float)GlobalConf::SCR_WIDTH /
                                                (float)GlobalConf::SCR_HEIGHT,
                                                0.1f, 500.0f);

        glm::mat4 view = camera->get_view_matrix();

        // render object
        // -------------
        WorldRenderer world_rd(world_shader, projection, view, light, clip_plane);
        world_rd.render(world);

        EntityRenderer tree_rd(tree_shader, projection, view, light);
        tree_rd.render({tree1, tree2, tree3, tree4, tree5, tree6, tree7, tree8});

        EntityRenderer rock_rd(rock_shader, projection, view, light);
        rock_rd.render({rock, rock2, rock3});

        glDisable(GL_CLIP_PLANE0);
        if (!fb) {
          WaterRenderer water_rd(water_shader, projection, view, light, camera->get_view_pos());
          water_rd.render(water);
        }

        // remove translation from the view matrix
        glDepthFunc(GL_LEQUAL);
        view = glm::mat4(glm::mat3(camera->get_view_matrix()));
        SkyboxRenderer skybox_rd(skybox_shader, projection, view);
        skybox_rd.render(skybox);
        glDepthFunc(GL_LESS);

        glEnable(GL_CLIP_PLANE0);
      };

    glEnable(GL_CLIP_PLANE0);

    // render reflection
    water.bind_reflection_fb();
    glm::vec3 old_camera_view_pos = camera->get_view_pos();
    float distance = 2 * (old_camera_view_pos.y  - water.get_height());
    camera->inc_view_pos(glm::vec3(0, -distance, 0));
    camera->invert_pitch();
    render_scene(glm::vec4{0.0f, 1.0f, 0.0f, -water.get_height()}, true);
    camera->inc_view_pos(glm::vec3(0, distance, 0));
    camera->invert_pitch();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // render refraction
    water.bind_refraction_fb();
    render_scene(glm::vec4{0.0f, -1.0f, 0.0f, water.get_height()}, true);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_CLIP_PLANE0);

    // render main scene
    render_scene(glm::vec4{0.0f, -1.0f, 0.0f, 10000});

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
