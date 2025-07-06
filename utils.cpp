#include <fstream>
#include <sstream>
#include <glad/glad.h>

#include "utils.h"
#include "stb_image.h"
#include <glm/glm.hpp>

#include "Camera.h"

extern glm::vec3 camera_position;
extern glm::vec3 camera_front;
extern glm::vec3 camera_up;

extern float delta_time;
extern float last_frame;

extern Camera camera;

float last_x = 400, last_y = 300;

bool first_mouse = true;

unsigned char* load_image(const std::string& filepath, int& width, int& height, int& n_channels)
{
    return stbi_load(filepath.c_str(), &width, &height, &n_channels, 0);
}

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.process_keyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.process_keyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.process_keyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.process_keyboard(RIGHT, delta_time);
}

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
    if (first_mouse)
    {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }
    
    float x_offset = x_pos - last_x;
    float y_offset = last_y - y_pos; // inverted: value increases from bottom to top; negative values rotate counter-clockwise.
    last_x = x_pos;
    last_y = y_pos;

    camera.process_mouse_movement(x_offset, y_offset);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    camera.process_mouse_scroll(static_cast<float>(y_offset));
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

glm::mat4 my_look_at(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
    // 1. Position = known
    // 2. Calculate cameraDirection
    glm::vec3 zaxis = glm::normalize(position - target);
    // glm::vec3 zaxis = camera.front;
    // 3. Get positive right axis vector
    glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
    // glm::vec3 xaxis = camera.right;
    // 4. Calculate camera up vector
    glm::vec3 yaxis = glm::cross(zaxis, xaxis);
    // glm::vec3 yaxis = camera.up;

    // Create translation and rotation matrix
    // In glm we access elements as mat[col][row] due to column-major layout
    glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
    translation[3][0] = -position.x; // Fourth column, first row
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = xaxis.x; // First column, first row
    rotation[1][0] = xaxis.y;
    rotation[2][0] = xaxis.z;
    rotation[0][1] = yaxis.x; // First column, second row
    rotation[1][1] = yaxis.y;
    rotation[2][1] = yaxis.z;
    rotation[0][2] = zaxis.x; // First column, third row
    rotation[1][2] = zaxis.y;
    rotation[2][2] = zaxis.z; 

    // Return lookAt matrix as combination of translation and rotation matrix
    return rotation * translation; // Remember to read from right to left (first translation then rotation)
}   