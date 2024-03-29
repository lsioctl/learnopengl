#include <vector>
#include <iostream>
#include <math.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VAO2.hpp"
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
  // We don't use EBO here, because of the texture coordinates (see world_coo2 result in this case)
  std::vector<float> vertices{
    // positions          // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  auto Nvertices{vertices.size()};

  auto vao{VAO2{vertices}};

  std::vector<glm::vec3> cube_position_list{
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

  // TODO: harcoded relative path
  auto main_shader{ShaderProgram{"./shaders/world_coo1_vtx.glsl", "./shaders/world_coo1_frag.glsl"}};
  auto main_shader_id{main_shader.id};

  // use texture units to bind multiple textures at once
  glActiveTexture(GL_TEXTURE0);
  texture_wood.bind();
  glActiveTexture(GL_TEXTURE1);
  texture_face.bind();

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

    std::size_t Ncube{cube_position_list.size()};

    for (std::size_t i = 0; i < Ncube; i++) {
      // Model matrix
      // Used to transform local (object coordinates) to world coordinates
      // always start with identity
      glm::mat4 model_matrix{glm::mat4(1.0f)};
      model_matrix = glm::translate(model_matrix, cube_position_list[i]);
      float angle{};
      if ( i % 3 == 0) {
        angle = (float)glfwGetTime();
      } else {
        angle = 20.0f * i;
      }
      model_matrix = glm::rotate(model_matrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
     
      // set the model in the shader
      glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));

      // render the cube
      glDrawArrays(
        GL_TRIANGLES,  // we want to draw triangles
        0,
        Nvertices  // we want this number of vertices in total
      );
    }

    

    // swap buffer and poll IO events
    glfwSwapBuffers(window);
    glfwPollEvents();    
  }

  glfwTerminate();

  return 0;
}