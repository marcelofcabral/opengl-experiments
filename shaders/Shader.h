#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.inl>

class Shader
{
public:
    GLuint id;

    Shader(const GLchar* vertex_path, const GLchar* fragment_path)
    {
        std::ifstream vertex_file;
        std::ifstream fragment_file;
        
        std::string vertex_code_string;
        std::string fragment_code_string;
        
        vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            vertex_file.open(vertex_path);
            fragment_file.open(fragment_path);
            
            std::stringstream vertex_stream, fragment_stream;

            vertex_stream << vertex_file.rdbuf();
            fragment_stream << fragment_file.rdbuf();

            vertex_file.close();
            fragment_file.close();

            vertex_code_string = vertex_stream.str();
            fragment_code_string = fragment_stream.str();
        }
        catch (std::ifstream::failure& error)
        {
            std::cout << "ERROR::SHADER::FILE_ERROR: " << error.what() << '\n';
        }

        const GLchar* vertex_code = vertex_code_string.c_str();
        const GLchar* fragment_code = fragment_code_string.c_str();

        GLuint vertex_shader_object, fragment_shader_object;
        
        GLint success;
        char info_log[512];

        vertex_shader_object = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader_object, 1, &vertex_code, nullptr);
        glCompileShader(vertex_shader_object);

        glGetShaderiv(vertex_shader_object, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(vertex_shader_object, 512, nullptr, info_log);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << "\n";
        }

        fragment_shader_object = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader_object, 1, &fragment_code, nullptr);
        glCompileShader(fragment_shader_object);

        glGetShaderiv(fragment_shader_object, GL_COMPILE_STATUS, &success);
        
        if (!success)
        {
            glGetShaderInfoLog(fragment_shader_object, 512, nullptr, info_log);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << "\n";
        }

        id = glCreateProgram();

        glAttachShader(id, vertex_shader_object);
        glAttachShader(id, fragment_shader_object);
        glLinkProgram(id);

        glGetProgramiv(id, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetProgramInfoLog(id, 512, nullptr, info_log);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << "\n";
        }

        glDeleteShader(vertex_shader_object);
        glDeleteShader(fragment_shader_object);
    }

    void use() const
    {
        glUseProgram(id);
    }
    
    void set_bool(const std::string &name, const bool value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
    }
    
    void set_int(const std::string &name, const int value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }
    
    void set_float(const std::string &name, const float value) const
    {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

    void set_vec3_f(const std::string &name, const std::vector<float> &value) const
    {
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, value.data());
    }

    void set_mat4(const std::string &name, const glm::mat4 &value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
};

#endif
