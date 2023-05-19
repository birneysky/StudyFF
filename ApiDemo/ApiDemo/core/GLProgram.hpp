//
//  GLProgram.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/12.
//

#ifndef GLProgram_hpp
#define GLProgram_hpp
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

class GLProgram {
private:
    GLuint program;
    GLuint vertShader;
    GLuint fragShader;
    int attributeCount = 0;
    std::map<std::string, GLuint> attributes;
private:
    bool compileShader(GLuint* shader, GLenum type, const std::string& source) {
        GLint status;
        const GLchar *code = source.c_str();
        if (!code) {
            std::cout << "Failed to load vertex shader" << std::endl;
            return false;
        }
        
        *shader = glCreateShader(type);
        glShaderSource(*shader, 1, &code, NULL);
        glCompileShader(*shader);
        
        glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
        
        if (status != GL_TRUE) {
            GLint logLength;
            glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
            if (logLength > 0) {
                GLchar *log = (GLchar *)malloc(logLength);
                glGetShaderInfoLog(*shader, logLength, &logLength, log);
                if (shader == &vertShader) {
                    std::cout << "vertex Shader log is : " << log << std::endl;
                } else {
                    std::cout << "fragment Shader log is : " << log << std::endl;
                }
                free(log);
            }
        }
        return status == GL_TRUE;
    }
public:
    GLProgram(const std::string& vShader, const std::string& fShader) {
        program = glCreateProgram();
        if (!compileShader(&vertShader, GL_VERTEX_SHADER, vShader)) {
            std::cout << "Failed to compile vertex shader" << std::endl;
        }
        
        if (!compileShader(&fragShader, GL_FRAGMENT_SHADER, fShader)) {
            std::cout << "Failed to compile fragment shader" << std::endl;
        }
        glAttachShader(program, vertShader);
        glAttachShader(program, fragShader);
    }
    
    bool link() {
        GLint status;
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {return false;}
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        
        return true;
    }
    
    void use() {
        glUseProgram(program);
    }
    
    void addAttribute(const std::string& name) {
        auto it = attributes.find(name);
        if (it == attributes.end()) {
            attributes.insert(std::make_pair(name, attributeCount));
            attributeCount ++;
        }
    }
    
    
    GLuint getAttributeIndex(const std::string& name) {
        return glGetAttribLocation(program, name.c_str());
    }
    
    GLuint getUniformIndex(const std::string& uniformName) {
        return glGetUniformLocation(program, uniformName.c_str());
    }
};

#endif /* GLProgram_hpp */
