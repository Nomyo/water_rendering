#include <opengl-utils.hh>
#include <cassert>
#include <functional>
#include <ctime>

void framebuffer_size_callback(GLFWwindow */*window*/, int width, int height)
{
  glViewport(0, 0, width, height);
}

unsigned int gen_texture(const std::string& file)
{
  unsigned texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);

  int width;
  int height;
  int nr_channels;

  stbi_set_flip_vertically_on_load(true);
  std::string tex = std::string{"textures/"} + file;
  unsigned char *data = stbi_load(tex.c_str(), &width, &height, &nr_channels, 0);

  assert(data);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
	       GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  return texture;
}

GLFWwindow *window_init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(GlobalConf::SCR_WIDTH,
					GlobalConf::SCR_HEIGHT,
					"GeneticWalk", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return NULL;
  }

  auto& global_conf = GlobalConf::get_instance();
  auto camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f),
			   glm::vec3(0.0f, 1.0f, 0.0f));

  global_conf.init(camera);

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, &GlobalConf::mouse_callback);
  glfwSetScrollCallback(window, &GlobalConf::scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return NULL;
  }

  glEnable(GL_DEPTH_TEST);
  return window;
}
