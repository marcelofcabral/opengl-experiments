#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"

#include "./utils.h"
#include "shaders/Shader.h"
#include "Camera.h"

float delta_time = 0.0f;
float last_frame = 0.0f;

enum class shader_type : uint8_t
{
    vertex,
    fragment
};

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cube_positions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);
    
    /*
    // vertex color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    */
    
    // vertex texture coordinate
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
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

    glEnable(GL_DEPTH_TEST);
    
    while (!glfwWindowShouldClose(window))
    {
        float current_frame = static_cast<float>(glfwGetTime());

        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        
        process_input(window);
        
        glClearColor(0.5f, 0.867f, 0.949f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, container_texture);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, face_texture);
        
        shader.use();
        glBindVertexArray(vao);
        
        // glm matrix operations
        // glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
        // glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);

        // camera z-axis
        // glm::vec3 camera_direction = camera_pos - camera_target; // inverse so the z-axis of the camera points towards z+

        // obtaining the right (x)-axis
        // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        // glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_direction));

        // obtaining the up (y)-axis
        // glm::vec3 camera_up = glm::normalize(glm::cross(camera_direction, camera_right));
        
        // glm::mat4 view_matrix = glm::mat4(1.f);

        // lookAt does the process of getting the up, right and direction vectors for us and creates a LookAt matrix (check lecture for info)
        // glm::mat4 view_matrix = glm::lookAt(camera_pos, camera_target, up);

        // making the camera 'go around in a circle' around the scene
        // const float radius = 10.f;
        // float cam_x = sin(static_cast<float>(glfwGetTime())) * radius;
        // float cam_z = cos(static_cast<float>(glfwGetTime())) * radius;
        // glm::mat4 view_matrix = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
        
        // view_matrix = glm::translate(view_matrix, glm::vec3(0.f, 0.f, -3.f));
        glm::mat4 projection_matrix = glm::perspective(glm::radians(camera.zoom), 800.f / 600.f, 0.1f, 100.f);
        shader.set_mat4("projection_matrix", projection_matrix);

        // glm::mat4 view_matrix = camera.get_view_matrix();
        glm::mat4 view_matrix = my_look_at(glm::vec3(camera.position.x, camera.position.y, camera.position.z), camera.position + camera.front, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.set_mat4("view_matrix", view_matrix);
        
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model_matrix = glm::mat4(1.f);
            model_matrix = glm::translate(model_matrix, cube_positions[i]);
            
            float angle = 20.f * static_cast<float>(i);

            if (i % 2 != 0)
            {
                angle = (float)glfwGetTime() * 25.f;
            }
            
            model_matrix = glm::rotate(model_matrix, glm::radians(angle), glm::vec3(1.f, 0.f, 0.5f));
            shader.set_mat4("model_matrix", model_matrix);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        /*
        glm::mat4 trans_left = glm::mat4(1.0f);

        trans_left = glm::translate(trans_left, glm::vec3(-0.5f, 0.5f, 0.f));
        
        trans_left = glm::scale(trans_left, glm::vec3(sin((float)glfwGetTime()) * 0.5f, sin((float)glfwGetTime()) * 0.5f, 1.f));
        
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(trans_left));
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    
    glfwTerminate();
    return 0;
}
