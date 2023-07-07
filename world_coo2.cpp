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

  // OpenGL stores all its depth inforamtion in z-buffer
  // glfw creates this buffer automatically for us (same as color buffer for the colors
  // of the output image)
  // depth is stored within each fragment (its z value)
  // and when a fragment want to output its color, it compare its depth value
  // with the z-buffer, if its behind and discards or overwrite the fragment accordingly
  // this process is called depth-testing
  // it is disabled by default in OpenGL, so enable it
  glEnable(GL_DEPTH_TEST);

  // vertices in normalized device coordinates (visible region of OpenGL)
  // EBO case: we do not duplicate the same point in memory but use indices
  // without it, for 2 triangles, we should have 6 vertices.
  std::vector<float> vertices{
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // front top right
     0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // front bottom right
    -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // front bottom left
    -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // front top left
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // back top right
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // back bottom right
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // back bottom left
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // back top left 
  };

  std::vector<unsigned int> vertices_indices{  // note that we start from 0!
    0, 1, 3,   // front first triangle
    1, 2, 3,    // front second triangle
    4, 5, 7,   // front first triangle
    5, 6, 7,    // front second triangle
    4, 5, 0,    // right first triangle
    5, 1, 0,     // right second triangle
    5, 1, 6,     // bottom first trianglee
    1, 6, 2,      // bottom second triangle
    3, 2, 7,     // left first
    2, 6, 7,     // left second
    4, 0, 7,    // top first
    0, 3, 7     // top second
  };

  auto Nvertices{vertices_indices.size()};

  auto vao{VAO{vertices, vertices_indices}};

  // TODO: harcoded relative path
  auto main_shader{ShaderProgram{"./shaders/world_coo1_vtx.glsl", "./shaders/world_coo1_frag.glsl"}};
  auto main_shader_id{main_shader.id};

  // use texture units to bind multiple textures at once
  glActiveTexture(GL_TEXTURE0);
  texture_wood.bind();
  glActiveTexture(GL_TEXTURE1);
  texture_face.bind();

  // Model matrix
  // Used to transform local (object coordinates) to world coordinates
  // always start with identity
  glm::mat4 model_matrix{glm::mat4(1.0f)};
  // we want or object to be rotated, 'face to the floor'
  model_matrix = glm::rotate(model_matrix, glm::radians(-50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  // View matrix
  // transform from world coordinate to 'camera' coordinates
  glm::mat4 view_matrix{glm::mat4(1.0f)};
  // we want the camera to move backward
  // OpenGL is a right handed system: x goes to the right, y to the top, and z toward us
  // note that we're translating the scene in the reverse direction of where we want to move
  view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, -3.0f));

  // Projection matrix
  // we want a standard perspective
  glm::mat4 projection_matrix{};
  // glm::perspective creates a 'frustrum' that define visible space
  // each coordinate inside this box will be mapped to a point in the clip space
  // the others not (they will be 'clipped')
  projection_matrix = glm::perspective(
    glm::radians(45.0f), // field of view
    800.0f / 600.0f, // aspect ratio, dividing the viewport width by its height
    0.1f, // near distance
    100.0f // far distance
  );

  // activate the shader
  main_shader.use();

  // Now shader is in use, we can set the uniforms for the texture units
  glUniform1i(glGetUniformLocation(main_shader_id, "our_texture"), 0); // set it manually
  main_shader.setInt("our_texture2", 1); // or with shader class

  int model_loc{glGetUniformLocation(main_shader_id, "model_matrix")};
  glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
  int view_loc{glGetUniformLocation(main_shader_id, "view_matrix")};
  glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
  int projection_loc{glGetUniformLocation(main_shader_id, "projection_matrix")};
  glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

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
    // clear the previous frame z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render the cube
    // We have a EBO instead of a VBO as a buffer target
    // so we replace glDrawArray by:
    glDrawElements(
      GL_TRIANGLES,  // we want to draw triangles
      Nvertices,  // we want this number of vertices in total
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