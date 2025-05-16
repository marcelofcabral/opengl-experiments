const char* vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragment_shader_source_left = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.54f, 0.12f, 0.67f, 1.0f);\n"
    "}\0";

const char* fragment_shader_source_right = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.082f, 0.859f, 0.745f, 1.0f);\n"
    "}\0";

enum class Tri
{
    Left,
    Right
};

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glad.c"

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

unsigned int create_vertex_shader()
{
    const unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    // assign shader source code to the newly created shader object
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);

    // check for compilation errors
    int success;
    char info_log[512];

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
        
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << '\n';
    }

    return vertex_shader;
}

unsigned int create_fragment_shader(const Tri tri)
{
    const unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragment_shader_source = tri == Tri::Left ? fragment_shader_source_left : fragment_shader_source_right;
    
    // assign shader source code to the newly created shader object
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);

    // check for compilation errors
    int success;
    char info_log[512];
    
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
        
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << '\n';
    }

    return fragment_shader;
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

    float left_tri_vertices[] = {
        -0.5f, 0.5f, 0.0f, // top tri 1
        -0.85f, -0.5f, 0.0f, // bottom left tri 1
        -0.15f, -0.5f, 0.0f, // bottom right tri 1
    };

    float right_tri_vertices[] = {
        0.5f, 0.5f, 0.0f, // top tri 2
        0.15f, -0.5f, 0.0f, // bottom left tri 2
        0.85f, -0.5f, 0.0f, // bottom right tri 2
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    // create Vertex Array Object to easily recover vertex attribute configurations of a Vertex Buffer Object when issuing a render call
    GLuint vaos[2], vbos[2];

    glGenVertexArrays(2, vaos);
    glGenBuffers(2, vbos);

    unsigned int ebo = 0;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // transfer indices data to the GPU memory
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    const unsigned int vertex_shader = create_vertex_shader();
    const unsigned int left_fragment_shader = create_fragment_shader(Tri::Left);
    const unsigned int right_fragment_shader = create_fragment_shader(Tri::Right);

    // create shader program and link shaders
    unsigned int left_shader_program = glCreateProgram();
    
    glAttachShader(left_shader_program, vertex_shader);
    glAttachShader(left_shader_program, left_fragment_shader);
    glLinkProgram(left_shader_program);
    
    unsigned int right_shader_program = glCreateProgram();
    
    glAttachShader(right_shader_program, vertex_shader);
    glAttachShader(right_shader_program, right_fragment_shader);
    glLinkProgram(right_shader_program);

    // check for linking errors
    // check for compilation errors
    int success;
    char info_log[512];
    
    glGetProgramiv(left_shader_program, GL_LINK_STATUS, &success);
    
    if (!success)
    {
        glGetProgramInfoLog(left_shader_program, 512, nullptr, info_log);
        
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << '\n';
    }

    glGetProgramiv(right_shader_program, GL_LINK_STATUS, &success);
    
    if (!success)
    {
        glGetProgramInfoLog(right_shader_program, 512, nullptr, info_log);
        
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << '\n';
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(left_fragment_shader);
    glDeleteShader(right_fragment_shader);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(left_tri_vertices), left_tri_vertices, GL_STATIC_DRAW);
    // bind data to the vao responsible for the left tri
    glBindVertexArray(vaos[0]);
    // bind vertex attribute 0 to the previously bound vbo, telling OpenGL how to interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
    
    // enable vertex attribute 0
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(right_tri_vertices), right_tri_vertices, GL_STATIC_DRAW);
    // bind data to the vao responsible for the right tri
    glBindVertexArray(vaos[1]);
    // bind vertex attribute 0 to the previously bound vbo, telling OpenGL how to interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
    
    // enable vertex attribute 0
    glEnableVertexAttribArray(0);
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    while (!glfwWindowShouldClose(window))
    {
        process_input(window);
        
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(left_shader_program);
        
        glBindVertexArray(vaos[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(right_shader_program);
        
        glBindVertexArray(vaos[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, vaos);
    glDeleteBuffers(2, vbos);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(left_shader_program);
    glDeleteProgram(right_shader_program);
    
    glfwTerminate();
    return 0;
}
