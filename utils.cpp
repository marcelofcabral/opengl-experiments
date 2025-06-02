#include <fstream>
#include <sstream>
#include <glad/glad.h>

#include "utils.h"
#include "stb_image.h"
#include "shaders/Shader.h"

unsigned char* load_image(const std::string& filepath, int& width, int& height, int& n_channels)
{
    return stbi_load(filepath.c_str(), &width, &height, &n_channels, 0);
}

void process_input(GLFWwindow* window, GLuint shader_program, float& curr_face_alpha)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        curr_face_alpha = clamp(curr_face_alpha + 0.05f, 0.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shader_program, "face_alpha"), curr_face_alpha);
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        curr_face_alpha = clamp(curr_face_alpha - 0.05f, 0.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shader_program, "face_alpha"), curr_face_alpha);
    }
}

std::string read_file(const std::string& filename)
{
    std::ifstream file(filename);
    
    if (!file.is_open()) return "";

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    
    return stream.str();
}

float clamp(const float value, const float min, const float max)
{
    return value < min ? min : value > max ? max : value;
}