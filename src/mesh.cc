#include <mesh.hh>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
	   std::vector<Texture> textures)
  : vertices_(vertices)
  , indices_(indices)
  , textures_(textures)
{
  setup_mesh();
}

void Mesh::draw(Shader shader)
{
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  unsigned int normalNr = 1;
  unsigned heightNr = 1;

  for (unsigned int i = 0; i < textures_.size(); ++i)
  {
    // Active right texture unit before binding
    glActiveTexture(GL_TEXTURE0 + i);

    std::stringstream ss;
    std::string number;
    std::string name = textures_[i].type;

    if(name == "texture_diffuse")
      ss << diffuseNr++; // transfer unsigned int to stream
    else if(name == "texture_specular")
      ss << specularNr++; // transfer unsigned int to stream
    else if(name == "texture_normal")
      ss << normalNr++; // transfer unsigned int to stream
    else if(name == "texture_height")
      ss << heightNr++; // transfer unsigned int to stream
    number = ss.str();

    glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
    glBindTexture(GL_TEXTURE_2D, textures_[i].id);
  }

  // Draw the mesh
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // Reset things back
  glActiveTexture(GL_TEXTURE0);
}

void Mesh::setup_mesh()
{
  // Create buffers / arrays
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  // Load data into vertex buffers
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex),
	       &vertices_[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int),
	       &indices_[0], GL_STATIC_DRAW);

  // Set the vertex attribute pointers

  // aPosition
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*)offsetof(Vertex, Normal));
  // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*)offsetof(Vertex, TexCoords));
  // vertex tangent
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*)offsetof(Vertex, Tangent));

  // vertex bitangent
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*)offsetof(Vertex, Bitangent));

  glBindVertexArray(0);
}

std::vector<Vertex> Mesh::get_vertices() const
{
  return vertices_;
}
