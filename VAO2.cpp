#include "VAO2.hpp"

VAO2::VAO2(const std::vector<float>& vertices)
{
  // Vertex Array Object
  // It will allow to store VBO and Vertex Attribute that
  // we will consequently declare
  // in Core OpenGL, VAO are mandatory
  glGenVertexArrays(1, &id);
  // once we bind it, all subsequent VBO and attributes
  // will be store on the VAO referenced by this id
  bind();

  // Vertex Buffer Object
  // will set up a memory on the graphic card
  // VBO will store its buffer id
  unsigned int VBO;
  glGenBuffers(1, &VBO);  
  // sGL_ARRAY_BUFFER is the type for a vertex buffer object
  glBindBuffer(GL_ARRAY_BUFFER, VBO); 
  // copy the vertex data into buffer memory for the currently
  // bound buffer (VBO)
  // the 4th parameters tells how we want the graphic card to manage the data
  // GL_STATIC_DRAW means the data is set up once and could be used many times
  // (so a memory region where read is fast could be good)
  // another example is GL_DYNAMIC_DRAW, which means thata data can change a lot
  // so it should be stored in a memory region where write (and read for usage) is fast
  // TODO: size dangerous if refactored: a wrong size will prevent any display
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  // Now we have to have to tell OpenGL how to interpret the raw data
  // vertices
  glVertexAttribPointer(
    0, // location of the vertex attribute, will be used in vertex shader with layout (location = 0)
    3, // vertex attribue is a vec3, 3 elements
    GL_FLOAT, // element of the vec3 is float
    GL_FALSE, // don't normalize data
    5 * sizeof(float), // size of a stride is 8 float elements
    (void*)0 // offset is 0
  );

  // vertex attributes are disabled by default, so enable the one we created
  glEnableVertexAttribArray(0);

  // textures
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  unbind();
}

void VAO2::bind()
{
  glBindVertexArray(id);
}


void VAO2::unbind()
{
  // unbind VAO by binding to the default one
  glBindVertexArray(0);
}
