#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum camera_movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    // euler Angles
    float yaw;
    float pitch;
    // camera options
    float movement_speed;
    float mouse_sensitivity;
    float zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(ZOOM)
    {
        this->position = position;
        world_up = up;
        this->yaw = yaw;
        this->pitch = pitch;
        update_camera_vectors();
    }
    // constructor with scalar values
    Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(ZOOM)
    {
        position = glm::vec3(pos_x, pos_y, pos_z);
        world_up = glm::vec3(up_x, up_y, up_z);
        this->yaw = yaw;
        this->pitch = pitch;
        update_camera_vectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 get_view_matrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void process_keyboard(camera_movement direction, float delta_time)
    {
        float velocity = movement_speed * delta_time;
        if (direction == FORWARD)
            position += glm::vec3(front.x, 0.f, front.z) * velocity;
        if (direction == BACKWARD)
            position -= glm::vec3(front.x, 0.f, front.z) * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void process_mouse_movement(float x_offset, float y_offset, GLboolean constrain_pitch = true)
    {
        x_offset *= mouse_sensitivity;
        y_offset *= mouse_sensitivity;

        yaw   += x_offset;
        pitch += y_offset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrain_pitch)
        {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        update_camera_vectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void process_mouse_scroll(float y_offset)
    {
        zoom -= (float)y_offset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void update_camera_vectors()
    {
        // calculate the new Front vector
        glm::vec3 front_vec;
        front_vec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front_vec.y = sin(glm::radians(pitch));
        front_vec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front_vec);
        // also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(front, world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up    = glm::normalize(glm::cross(right, front));
    }
};
#endif