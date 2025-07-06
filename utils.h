#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>

unsigned char* load_image(const std::string& filepath, int& width, int& height, int& n_channels);

void process_input(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

std::string read_file(const std::string& filename);

float clamp(const float value, const float min, const float max);

glm::mat4 my_look_at(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp);

#endif // UTILS_H
