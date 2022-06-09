#include <iostream>
//#include <math.h>
#include <fstream>
#include <sstream>

#include "glad/glad.h"
//#include "GLFW/glfw3.h"

#include "ShaderProgram.h"

void ShaderProgram::checkCompilationStatus_(int vertex_shader_id)
{
  int success;
  char info_log[512];
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);

  if (success == false)
  {
    glGetShaderInfoLog(vertex_shader_id, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
  }
}

void ShaderProgram::checkLinkingStatus_(int shader_program_id)
{
  int success;
  char info_log[512];
  glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);

  if (success == false)
  {
    glGetProgramInfoLog(shader_program_id, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
  }
}

void ShaderProgram::readFromFile_(const char* file_path, std::string& dest_string)
{
  std::ifstream file_stream;
  // ensure ifstream objects can throw exceptions:
  file_stream.exceptions (std::ifstream::failbit | std::ifstream::badbit);

  try 
  {
    // open files
    file_stream.open(file_path);
    std::stringstream string_stream;
    // read file's buffer contents into streams
    string_stream << file_stream.rdbuf();
    // close file handlers
    file_stream.close();
    // convert stream into string
    dest_string = string_stream.str();	
  }
  catch (std::ifstream::failure e)
  {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }
}

ShaderProgram::ShaderProgram(const char* vertex_path, const char* fragment_path)
{
  // "Modern" OpenGL wants us to define 2 shaders: vertex and fragments
  // vertex shader is the first one in the pipeline
  std::string temp_vertex_source;
  readFromFile_(vertex_path, temp_vertex_source);
  const char *vertex_shader_source{temp_vertex_source.c_str()};

  // create a shader object and get its id
  vertex_shader_id_ = glCreateShader(GL_VERTEX_SHADER);
  // attach and compile
  glShaderSource(vertex_shader_id_, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader_id_);

  checkCompilationStatus_(vertex_shader_id_);

  // Fragment shader handling
  // fragment shader handles the color output of the pixels
  // color in GLSL is in RGBA (Alpha: opacity)
  // uniform are *global* for a shader program, they are shared
  // by each shader of the program
  std::string temp_fragment_source;
  readFromFile_(fragment_path, temp_fragment_source);
  const char *fragment_shader_source{temp_fragment_source.c_str()};

  fragment_shader_id_ = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader_id_, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader_id_);

  checkCompilationStatus_(fragment_shader_id_);


  // Create the shader programm by linking the two shaders
  id = glCreateProgram();
  glAttachShader(id, vertex_shader_id_);
  glAttachShader(id, fragment_shader_id_);
  glLinkProgram(id);

  checkLinkingStatus_(id);

  // Clean the shader objects, they are not in use anymore
  glDeleteShader(vertex_shader_id_);
  glDeleteShader(fragment_shader_id_);
}

void ShaderProgram::use()
{
  // Activate this programm for each render and shader call
  glUseProgram(id);
}

void ShaderProgram::setUniformBool(const std::string &uniform_name, bool uniform_value)
{
  glUniform1i(glGetUniformLocation(id, uniform_name.c_str()), static_cast<int>(uniform_value)); 
}

void ShaderProgram::setUniformInt(const std::string &uniform_name, int uniform_value)
{ 
    glUniform1i(glGetUniformLocation(id, uniform_name.c_str()), uniform_value); 
}

void ShaderProgram::setUniformFloat(const std::string &uniform_name, float uniform_value)
{ 
    glUniform1f(glGetUniformLocation(id, uniform_name.c_str()), uniform_value); 
}