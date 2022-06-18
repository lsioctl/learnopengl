#include <iostream>
#include <math.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "ShaderProgram.hpp"
#include "Texture.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void printNVertexAttribute()
{
  int nVertexAttributes{0};
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nVertexAttributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nVertexAttributes << std::endl;
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
  }
  glfwMakeContextCurrent(window);

  // Glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
  }

  auto texture_wood{Texture{"./textures/container.jpg", GL_RGB}};
  auto texture_face{Texture{"./textures/awesomeface.png", GL_RGBA}};

  // First two parametres set the location of the lower left corner
  // of the window.
  // Note2: viewport could be smaller than window, so we could render
  // things outside the viewport
  // OpenGL use the view port to translate it's 2D coordinates
  // to coordinate on the window. eg (-0.5, 0.5) => (200, 450)
  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); 

  // vertices in normalized device coordinates (visible region of OpenGL)
  // EBO case: we do not duplicate the same point in memory but use indices
  // without it, for 2 triangles, we should have 6 vertices.
  float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
  };

  unsigned int vertices_indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
  };

  // Vertex Array Object
  // It will allow to store VBO and Vertex Attribute that
  // we will consequently declare
  // in Core OpenGL, VAO are mandatory
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // once we bind it, all subsequent VBO and attributes
  // will be store on the VAO referenced by this id
  glBindVertexArray(VAO);

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
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // EBO: Element Buffer Object, store the indices we want to draw in memory
  unsigned int EBO;
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices_indices), vertices_indices, GL_STATIC_DRAW);

  // Now we have to have to tell OpenGL how to interpret the raw data
  glVertexAttribPointer(
    0, // location of the vertex attribute, will be used in vertex shader with layout (location = 0)
    3, // vertex attribue is a vec3, 3 elements
    GL_FLOAT, // element of the vec3 is float
    GL_FALSE, // don't normalize data
    8 * sizeof(float), // size of a stride is 8 float elements
    (void*)0 // offset is 0
  );

  // vertex attributes are disabled by default, so enable the one we created
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(1);

  printNVertexAttribute();

  // TODO: harcoded relative path
  auto main_shader{ShaderProgram{"./shaders/texture3_vtx.glsl", "./shaders/texture3_frag.glsl"}};
  auto main_shader_id{main_shader.id};

  // use texture units to bind multiple textures at once
  glActiveTexture(GL_TEXTURE0);
  texture_wood.bind();
  glActiveTexture(GL_TEXTURE1);
  texture_face.bind();

  // activate the shader
  main_shader.use();

  // Now shader is in use, we can set the uniforms for the texture units
  glUniform1i(glGetUniformLocation(main_shader_id, "our_texture"), 0); // set it manually
  main_shader.setUniformInt("our_texture2", 1); // or with shader class

  // Render loop
  while(!glfwWindowShouldClose(window))
  {
    processInput(window);

    // rendering commands here
    // state-setting function
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // state-using function
    glClear(GL_COLOR_BUFFER_BIT);

    // render the rectangle
    glBindVertexArray(VAO);
    // We have a EBO instead of a VBO as a buffer target
    // so we replace glDrawArray by:
    glDrawElements(
      GL_TRIANGLES,  // we want to draw triangles
      6,  // we want 6 vertices in total
      GL_UNSIGNED_INT, // type of the indices
      0 // EBO offset, or pass in index array if we do not use EBO
    );

    // swap buffer and poll IO events
    glfwSwapBuffers(window);
    glfwPollEvents();    
  }

  glfwTerminate();

  return 0;
}