//
//  i420Shader.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/16.
//

#ifndef i420Shader_hpp
#define i420Shader_hpp

#include <stdio.h>



#include "ShadingHelper.h"
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#include <OpenGLES/gltypes.h>
#include <string>
static const int kYTextureUnit0 = 0;
static const int kUTextureUnit1 = 1;
static const int kVTextureUnit2 = 2;


class I420Shader {
private:
    GLuint _i420Program;
    GLint  _position;
    GLint _textureCoord;
    
    
    const std::string vertexShader = R"(
        attribute vec4 position;
        attribute vec2 texcoord;
        varying vec2 v_texcoord;
     
        void main()
        {
            gl_Position = position;
            v_texcoord = texcoord.xy;
        }
    )";
    const std::string fragmentShader = R"(
        varying highp vec2 v_texcoord;
        uniform lowp sampler2D s_textureY;
        uniform lowp sampler2D s_textureU;
        uniform lowp sampler2D s_textureV;

        void main() {
            mediump vec3 yuv;
            yuv.x = texture2D(s_textureY, v_texcoord).r - 0.0625;
            yuv.y = texture2D(s_textureU, v_texcoord).r - 0.5;
            yuv.z = texture2D(s_textureV, v_texcoord).r - 0.5;

            mediump mat3 matrix = mat3( 1.0, 1.0,1.0,
                                        0.0, -0.18732, 1.8556,
                                        1.57481,-0.46813,0.0);
            lowp vec3 rgb = matrix * yuv;
            gl_FragColor = vec4(rgb, 1.0);
        }
    )";
    
private:
    bool buildProgram() {
        bool result = false;
        GLuint vertShader = 0, fragShader = 0;
        _i420Program = glCreateProgram();
        vertShader = compileShader(GL_VERTEX_SHADER, (GLchar*)vertexShader.c_str());
        //NSAssert(vertShader != 0, @"compile vertex shader failed");
        
        fragShader = compileShader(GL_FRAGMENT_SHADER, (GLchar*)fragmentShader.c_str());
        //NSAssert(fragShader != 0, @"compile fragment shader failed");
        
        glAttachShader(_i420Program, vertShader);
        glAttachShader(_i420Program, fragShader);
        
        glLinkProgram(_i420Program);
        
        _position = glGetAttribLocation(_i420Program, "position");
        _textureCoord = glGetAttribLocation(_i420Program, "texcoord");
        
        
        GLint status;
        glGetProgramiv(_i420Program, GL_LINK_STATUS, &status);
        //NSAssert(status == GL_TRUE, @"Failed to link program %d", _nv12Program);
        
        result = validateProgram(_i420Program);
        //NSAssert(result, @"etup GL programm failed");
        glUseProgram(_i420Program);
        
        GLint ySampler = glGetUniformLocation(_i420Program, "s_textureY");
        GLint uSampler = glGetUniformLocation(_i420Program, "s_textureU");
        GLint vSampler = glGetUniformLocation(_i420Program, "s_textureV");
        
        //        NSAssert(ySampler >= 0, @"Failed to get uniform variable locations in NV12 shader");
        //        NSAssert(uvSampler >= 0, @"Failed to get uniform variable locations in NV12 shader");
        
        
        glUniform1i(ySampler, kYTextureUnit0);
        glUniform1i(uSampler, kUTextureUnit1);
        glUniform1i(vSampler, kVTextureUnit2);
//
        return result;
    }
    
    
public:
    I420Shader() {
        
    }
    
    ~I420Shader() {
        glDeleteProgram(_i420Program);
        _i420Program = 0;
    }
    
    void applay(int width, int height, GLuint* textures, int len) {
        if (!_i420Program) {
            buildProgram();
        }
 
        static const GLfloat imageVertices[] = {
            1.0f, 1.0f,
            -1.0, 1.0,
            1.0, -1.0,
            -1.0, -1.0
        };
        
        GLfloat noRotationTextureCoordinates[] = {
            1.0, 1.0,
            0.0, 1.0,
            1.0, 0.0,
            0.0, 0.0
        };
        
        
        glUseProgram(_i420Program);
        glVertexAttribPointer(_position, 2, GL_FLOAT, 0, 0, imageVertices);
        glEnableVertexAttribArray(_position);
        glVertexAttribPointer(_textureCoord, 2, GL_FLOAT, 0, 0, noRotationTextureCoordinates);
        glEnableVertexAttribArray(_textureCoord);
        
        

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
     
        for (int i = 0; i < len; i++) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + i));
            glBindTexture(GL_TEXTURE_2D, textures[i]);
        }
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glUseProgram(0);
    }
};
        

#endif /* i420Shader_hpp */
