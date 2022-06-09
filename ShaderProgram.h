#pragma once

#include <string>

class ShaderProgram {
private:
  int vertex_shader_id_;
  int fragment_shader_id_;
  void readFromFile_(const char* file_path, std::string& dest_string);
  void checkCompilationStatus_(int shader_id);
  void checkLinkingStatus_(int shader_program_id);
public:
  ShaderProgram(const char* vertex_path, const char* fragment_path);
  int id;
  void use();
  void setUniformBool(const std::string &uniform_name, bool uniform_value);
  void setUniformInt(const std::string &uniform_name, int uniform_value);
  void setUniformFloat(const std::string &uniform_name, float uniform_value);
};