#pragma once

#include <string>

#include "glad/glad.h"

class ShaderProgram {
private:
  GLuint vertex_shader_id_;
  GLuint fragment_shader_id_;
  void readFromFile_(const char* file_path, std::string& dest_string);
  void checkCompilationStatus_(int shader_id);
  void checkLinkingStatus_(int shader_program_id);
  GLuint compile_(const std::string& shader_source, GLenum gl_shader_type);
public:
  ShaderProgram(const char* vertex_path, const char* fragment_path);
  GLuint id;
  void use();
  void setUniformBool(const std::string &uniform_name, bool uniform_value);
  void setUniformInt(const std::string &uniform_name, int uniform_value);
  void setUniformFloat(const std::string &uniform_name, float uniform_value);
};