#include <iostream>
#include <vector>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./utils.h"
#include "glad.c"
#include "shaders/Shader.h"

enum class shader_type : uint8_t
{
    vertex,
    fragment
};

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLuint create_shader(const shader_type shader_type, const GLchar* shader_source)
{
    const GLuint shader_object = glCreateShader(shader_type == shader_type::vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

    // assign shader source code to the newly created shader object
    glShaderSource(shader_object, 1, &shader_source, nullptr);
    glCompileShader(shader_object);

    // check for compilation errors
    int success;
    char info_log[512];

    glGetShaderiv(shader_object, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader_object, 512, nullptr, info_log);
        
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << '\n';
    }

    return shader_object;
}

void update_rgb_values(std::vector<GLfloat>& rgb_values, size_t& current_index)
{
    std::cout << "current value: " << rgb_values[current_index] << '\n';
    if (rgb_values[current_index] < 1e-5f)
    {
        current_index = (current_index + 1) % rgb_values.size();
    }

    const GLfloat color_value = clamp((static_cast<float>(sin(glfwGetTime() * 1.5f)) * 0.5f + 0.5f), 0.f, 1.f);
    
    rgb_values[current_index] = color_value;
}

int main(int argc, char* argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

    if (!window)
    {
        std::cout << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << '\n';
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    /*
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  // bottom left
       -0.5f,  0.5f, 0.0f   // top left 
    };
    */
    /*
    constexpr float vertices[] = {
        -0.5f, 0.5f, 0.0f, // top tri 1
        -0.85f, -0.5f, 0.0f, // bottom left tri 1
        -0.15f, -0.5f, 0.0f, // bottom right tri 1

        0.5f, 0.5f, 0.0f, // top tri 2
        0.15f, -0.5f, 0.0f, // bottom left tri 2
        0.85f, -0.5f, 0.0f, // bottom right tri 2
    };
    */

    float vertices[] = {
        // positions        // colors
        0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };
    
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    
    // create Vertex Array Object to easily recover vertex attribute configurations of a Vertex Buffer Object when issuing a render call
    GLuint vao, vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    unsigned int ebo = 0;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // transfer indices data to the GPU memory
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    const std::string vertex_shader_source = read_file("./shaders/vertex.glsl");
    const std::string fragment_shader_source = read_file("./shaders/fragment.glsl");
    
    const GLuint vertex_shader = create_shader(shader_type::vertex, vertex_shader_source.c_str());
    const GLuint fragment_shader = create_shader(shader_type::fragment, fragment_shader_source.c_str());

    // create shader program and link shaders
    const GLuint shader_program = glCreateProgram();
    
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    // check for linking errors
    int success;
    char info_log[512];
    
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
        
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << '\n';
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // bind data to the vao
    glBindVertexArray(vao);
    // bind vertex attribute 0 to the previously bound vbo, telling OpenGL how to interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // std::vector<float> rgb_values{0.f, 0.f, 0.f};
    // size_t current_index = 2;

    Shader shader{"./shaders/vertex.glsl", "./shaders/fragment.glsl"};
    
    while (!glfwWindowShouldClose(window))
    {
        process_input(window);
        
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // update_rgb_values(rgb_values, current_index);

        // const GLint color_location = glGetUniformLocation(shader_program, "our_color");
        
        // glUseProgram(shader_program);

        shader.use();

        // glUniform4fv(color_location, 1, rgb_values.data());
        
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shader_program);
    
    glfwTerminate();
    return 0;
}
