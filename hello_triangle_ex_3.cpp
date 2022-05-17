#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

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

void checkCompilationStatus(int vertex_shader_id)
{
  int  success;
  char info_log[512];
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);

  if(!success)
  {
    glGetShaderInfoLog(vertex_shader_id, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
  }
}

void checkLinkingStatus(int shader_program_id)
{
  int  success;
  char info_log[512];
  glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);

  if(!success)
  {
    glGetProgramInfoLog(shader_program_id, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
  }
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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
  }

  // First two parametres set the location of the lower left corner
  // of the window.
  // Note2: viewport could be smaller than window, so we could render
  // things outside the viewport
  // OpenGL use the view port to translate it's 2D coordinates
  // to coordinate on the window. eg (-0.5, 0.5) => (200, 450)
  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); 

  // vertices in normalized device coordinates (visible region of OpenGL)
  float triangle1_vertices[] = {
    -1.0f, -0.75f, 0.0f,
     0.0f, -0.75f, 0.0f,
    -0.5f,  0.0f, 0.0f
  };

  float triangle2_vertices[] = {
     0.0f,  0.5f, 0.0f,
     1.0f,  0.5f, 0.0f,
     0.5f,  0.0f, 0.0f
  };

  // Vertex Array Object
  // It will allow to store VBO and Vertex Attribute that
  // we will consequently declare
  // in Core OpenGL, VAO are mandatory
  unsigned int VAO1;
  glGenVertexArrays(1, &VAO1);
  // once we bind it, all subsequent VBO and attributes
  // will be store on the VAO referenced by this id
  glBindVertexArray(VAO1);

  // Vertex Buffer Object
  // will set up a memory on the graphic card
  // VBO will store its buffer id
  unsigned int VBO1;
  glGenBuffers(1, &VBO1);  
  // sGL_ARRAY_BUFFER is the type for a vertex buffer object
  glBindBuffer(GL_ARRAY_BUFFER, VBO1); 
  // copy the vertex data into buffer memory for the currently
  // bound buffer (VBO)
  // the 4th parameters tells how we want the graphic card to manage the data
  // GL_STATIC_DRAW means the data is set up once and could be used many times
  // (so a memory region where read is fast could be good)
  // another example is GL_DYNAMIC_DRAW, which means thata data can change a lot
  // so it should be stored in a memory region where write (and read for usage) is fast
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1_vertices), triangle1_vertices, GL_STATIC_DRAW);

  // Now we have to have to tell OpenGL how to interpret the raw data
  glVertexAttribPointer(
    0, // location of the vertex attribute, will be used in vertex shader with layout (location = 0)
    3, // vertex attribue is a vec3, 3 elements
    GL_FLOAT, // element of the vec3 is float
    GL_FALSE, // don't normalize data
    3 * sizeof(float), // size of a stride is 3 float elements
    (void*)0 // offset is 0
  );

  // vertex attributes are disabled by default, so enable the one we created
  glEnableVertexAttribArray(0);

  // Vertex Array Object
  // It will allow to store VBO and Vertex Attribute that
  // we will consequently declare
  // in Core OpenGL, VAO are mandatory
  unsigned int VAO2;
  glGenVertexArrays(1, &VAO2);
  // once we bind it, all subsequent VBO and attributes
  // will be store on the VAO referenced by this id
  glBindVertexArray(VAO2);

  // Vertex Buffer Object
  // will set up a memory on the graphic card
  // VBO will store its buffer id
  unsigned int VBO2;
  glGenBuffers(1, &VBO2);  
  // sGL_ARRAY_BUFFER is the type for a vertex buffer object
  glBindBuffer(GL_ARRAY_BUFFER, VBO2); 
  // copy the vertex data into buffer memory for the currently
  // bound buffer (VBO)
  // the 4th parameters tells how we want the graphic card to manage the data
  // GL_STATIC_DRAW means the data is set up once and could be used many times
  // (so a memory region where read is fast could be good)
  // another example is GL_DYNAMIC_DRAW, which means thata data can change a lot
  // so it should be stored in a memory region where write (and read for usage) is fast
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2_vertices), triangle2_vertices, GL_STATIC_DRAW);

  // Now we have to have to tell OpenGL how to interpret the raw data
  glVertexAttribPointer(
    0, // location of the vertex attribute, will be used in vertex shader with layout (location = 0)
    3, // vertex attribue is a vec3, 3 elements
    GL_FLOAT, // element of the vec3 is float
    GL_FALSE, // don't normalize data
    3 * sizeof(float), // size of a stride is 3 float elements
    (void*)0 // offset is 0
  );

  // vertex attributes are disabled by default, so enable the one we created
  glEnableVertexAttribArray(0);

  // "Modern" OpenGL wants us to define 2 shaders: vertex and fragments
  // vertex shader is the first one in the pipeline
  const char *vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 a_pos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);\n"
    "}\0";

  // create a shader object and get its id
  unsigned int vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  // attach and compile
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);

  checkCompilationStatus(vertex_shader);

  // Fragment shader handling
  // fragment shader handles the color output of the pixels
  // color in GLSL is in RGBA (Alpha: opacity)
  const char* fragment_shader_source1 = "#version 330 core\n"
    "out vec4 frag_color;\n"
    "void main()\n"
    "{\n"
    "  frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "} \0";

  unsigned int fragment_shader1;
  fragment_shader1 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader1, 1, &fragment_shader_source1, NULL);
  glCompileShader(fragment_shader1);

  checkCompilationStatus(fragment_shader1);


  // Fragment shader handling
  // fragment shader handles the color output of the pixels
  // color in GLSL is in RGBA (Alpha: opacity)
  const char* fragment_shader_source2 = "#version 330 core\n"
    "out vec4 frag_color;\n"
    "void main()\n"
    "{\n"
    "  frag_color = vec4(1.f, 1.f, 0.0f, 1.f);\n"
    "} \0";

  unsigned int fragment_shader2;
  fragment_shader2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader2, 1, &fragment_shader_source2, NULL);
  glCompileShader(fragment_shader2);

  checkCompilationStatus(fragment_shader2);


  // Create the shader programm by linking the two shaders
  unsigned int shader_program1;
  shader_program1 = glCreateProgram();
  glAttachShader(shader_program1, vertex_shader);
  glAttachShader(shader_program1, fragment_shader1);
  glLinkProgram(shader_program1);

  checkLinkingStatus(shader_program1);

  unsigned int shader_program2;
  shader_program2 = glCreateProgram();
  glAttachShader(shader_program2, vertex_shader);
  glAttachShader(shader_program2, fragment_shader2);
  glLinkProgram(shader_program2);

  checkLinkingStatus(shader_program2);

  // Clean the shader objects, they are not in use anymore
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader1);
  glDeleteShader(fragment_shader2);

  
  // Render loop
  while(!glfwWindowShouldClose(window))
  {
    processInput(window);

    // rendering commands here
    // state-setting function
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // state-using function
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program1);
    glBindVertexArray(VAO1);
    glDrawArrays(
      GL_TRIANGLES,  // we want to draw triangles
      0,  // starting index of the vertex array we want to draw (?)
      3 // we want to draw 3 vertices
    );
    glUseProgram(shader_program2);
    glBindVertexArray(VAO2);
    glDrawArrays(
      GL_TRIANGLES,  // we want to draw triangles
      0,  // starting index of the vertex array we want to draw (?)
      3 // we want to draw 3 vertices
    );

    glfwSwapBuffers(window);
    glfwPollEvents();    
  }

  glfwTerminate();

  return 0;
}