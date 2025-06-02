#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <GLFW/glfw3.h>

unsigned char* load_image(const std::string& filepath, int& width, int& height, int& n_channels);

void process_input(GLFWwindow* window, GLuint shader_program, float& curr_face_alpha);

std::string read_file(const std::string& filename);

float clamp(const float value, const float min, const float max);

#endif // UTILS_H
