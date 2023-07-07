#include <vector>
#include <iostream>
#include <math.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VAO.hpp"
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
  std::vector<float> vertices{
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
  };

  std::vector<unsigned int> vertices_indices{  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
  };

  auto vao{VAO{vertices, vertices_indices}};

  // TODO: harcoded relative path
  auto main_shader{ShaderProgram{"./shaders/rotate1_vtx.glsl", "./shaders/rotate1_frag.glsl"}};
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
  main_shader.setInt("our_texture2", 1); // or with shader class

  vao.bind();

  // Render loop
  while(!glfwWindowShouldClose(window))
  {
    processInput(window);

    // rendering commands here
    // state-setting function
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // state-using function
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 trans_matrix = glm::mat4(1.0f);
    trans_matrix = glm::translate(trans_matrix, glm::vec3(0.5f, -0.5f, 0.0f));
    trans_matrix = glm::rotate(trans_matrix, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

    // Note: I should enforce shader.use() here to be sur uniform bound on the right
    // shader
    // Send the tranform matrix to the shader via uniforms
    unsigned int transformLoc = glGetUniformLocation(main_shader_id, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans_matrix));

    // render the rectangle
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