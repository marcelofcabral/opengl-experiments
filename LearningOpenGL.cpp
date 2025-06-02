#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "./utils.h"
#include "shaders/Shader.h"


enum class shader_type : uint8_t
{
    vertex,
    fragment
};

namespace
{
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
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
        // positions        // colors
        0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };
    */

    
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
   };
    
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    
    // create Vertex Array Object to easily recover vertex attribute configurations of a Vertex Buffer Object when issuing a render call
    GLuint vao, vbo, ebo;
    GLuint container_texture, face_texture;
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // transfer indices data to the GPU memory
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // load textures
    stbi_set_flip_vertically_on_load(true);
    int img_width, img_height, img_channels;

    // container texture
    glGenTextures(1, &container_texture);
    
    glBindTexture(GL_TEXTURE_2D, container_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    unsigned char* container_image_data = load_image("./assets/container.jpg", img_width, img_height, img_channels);

    if (container_image_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, container_image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load container image" << '\n';
    }

    // face texture
    glGenTextures(1, &face_texture);
    
    glBindTexture(GL_TEXTURE_2D, face_texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    unsigned char* face_image_data = load_image("./assets/awesomeface.png", img_width, img_height, img_channels);

    if (face_image_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, face_image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load face image" << '\n';
    }

    stbi_image_free(face_image_data);
    stbi_image_free(container_image_data);
    
    // bind data to the vao
    glBindVertexArray(vao);
    
    // bind vertex attribute 0 to the previously bound vbo, telling OpenGL how to interpret vertex data
    // vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    // vertex color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // vertex texture coordinate
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    const Shader shader{"./shaders/vertex.glsl", "./shaders/fragment.glsl"};

    shader.use();
    shader.set_int("container_texture", 0);
    shader.set_int("face_texture", 1);

    float face_alpha = 0.f;
    
    while (!glfwWindowShouldClose(window))
    {
        process_input(window, shader.id, face_alpha);
        
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, container_texture);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, face_texture);
        
        shader.use();
        glBindVertexArray(vao);
        
        // glm operations
        glm::mat4 trans_right = glm::mat4(1.0f);

        // operations are done from right to left. So the first operation is scaling, then rotation
        // rotate around z-axis, which is the axis orthogonal to the screen plane
        // trans_right = glm::rotate(trans_right, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
        
        // trans_rightlate to the bottom right corner of the viewport
        trans_right = glm::translate(trans_right, glm::vec3(0.5f, -0.5f, 0.f));
        
        // rotate around the z-axis, the rotation will be updated over time
        trans_right = glm::rotate(trans_right, glm::radians((float)glfwGetTime() * 5.f), glm::vec3(0.f, 0.f, 1.f));
    
        // make the box twice as small
        trans_right = glm::scale(trans_right, glm::vec3(0.5f, 0.5f, 0.5f));

        const GLint transform_location = glGetUniformLocation(shader.id, "transform");
    
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(trans_right));
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glm::mat4 trans_left = glm::mat4(1.0f);

        trans_left = glm::translate(trans_left, glm::vec3(-0.5f, 0.5f, 0.f));
        
        trans_left = glm::scale(trans_left, glm::vec3(sin((float)glfwGetTime()) * 0.5f, sin((float)glfwGetTime()) * 0.5f, 1.f));
        
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(trans_left));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    
    glfwTerminate();
    return 0;
}
