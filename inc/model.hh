#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.hh>
#include <shader_m.hh>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

unsigned int TextureFromFile(const std::string& file,
			     bool gamma);

class Model
{
public:
  Model(const std::string& model_path, bool gamma = false);
  Model(const std::string& model_path, const std::string& texture_path,
	const std::string& directory, bool gamma);
  void draw(Shader shader);

private:
  void load_model(std::string const &path);
  void processNode(aiNode *node, const aiScene *scene);

  Mesh processMesh(aiMesh *mesh, const aiScene *scene);

  // check all material textures of a given type
  // Load textures only if they're note already loaded
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
					    aiTextureType type,
					    std::string typeName);

private:
  std::vector<Texture> textures_loaded_;
  std::vector<Mesh> meshes_;

  std::string model_path_;
  std::string texture_path_;
  std::string directory_;

  bool gamma_correction_;
};
