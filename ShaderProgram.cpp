#include <iostream>
//#include <math.h>
#include <fstream>
#include <sstream>

#include "glad/glad.h"
//#include "GLFW/glfw3.h"

#include "ShaderProgram.hpp"

void ShaderProgram::checkCompilationStatus_(int vertex_shader_id)
{
  int success;
  char info_log[512];
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);

  if (success == false)
  {
    glGetShaderInfoLog(vertex_shader_id, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
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

GLuint ShaderProgram::compile_(const std::string& shader_source, GLenum gl_shader_type)
{
  const char *shader_source_c{shader_source.c_str()};

  // create a shader object and get its id
  GLuint shader_id{glCreateShader(gl_shader_type)};
  // attach and compile
  glShaderSource(shader_id, 1, &shader_source_c, NULL);
  glCompileShader(shader_id);

  return shader_id;
}

ShaderProgram::ShaderProgram(const char* vertex_path, const char* fragment_path)
{
  // "Modern" OpenGL wants us to define 2 shaders: vertex and fragments
  // vertex shader is the first one in the pipeline
  std::string temp_vertex_source;
  readFromFile_(vertex_path, temp_vertex_source);

  vertex_shader_id_ = compile_(temp_vertex_source, GL_VERTEX_SHADER);

  checkCompilationStatus_(vertex_shader_id_);

  // Fragment shader handling
  // fragment shader handles the color output of the pixels
  // color in GLSL is in RGBA (Alpha: opacity)
  // uniform are *global* for a shader program, they are shared
  // by each shader of the program
  std::string temp_fragment_source;
  readFromFile_(fragment_path, temp_fragment_source);

  fragment_shader_id_ = compile_(temp_fragment_source, GL_FRAGMENT_SHADER);

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

GLint ShaderProgram::getUniformLocation_(const std::string& uniform_name) {
  return glGetUniformLocation(id, uniform_name.c_str());
}

void ShaderProgram::setUniformBool(const std::string& uniform_name, bool uniform_value)
{
  glUniform1i(getUniformLocation_(uniform_name), static_cast<int>(uniform_value));
}

void ShaderProgram::setUniformInt(const std::string& uniform_name, int uniform_value)
{ 
    glUniform1i(getUniformLocation_(uniform_name), uniform_value);
}

void ShaderProgram::setUniformFloat(const std::string &uniform_name, float uniform_value)
{ 
    glUniform1f(getUniformLocation_(uniform_name), uniform_value);
}

void ShaderProgram::setMat3(const std::string& uniform_name, const glm::mat3& mat) {
  glUniformMatrix3fv(
    getUniformLocation_(uniform_name),
    1,
    GL_FALSE,
    glm::value_ptr(mat)
  );
}

void ShaderProgram::setMat4(const std::string& uniform_name, const glm::mat4& mat) {
  glUniformMatrix4fv(
    getUniformLocation_(uniform_name),
    1,
    GL_FALSE,
    glm::value_ptr(mat)
  );
}

void ShaderProgram::setVec3(const std::string &uniform_name, const glm::vec3& vec) {
  glUniform3fv(
    getUniformLocation_(uniform_name),
    1,
    &vec[0]
  );
}