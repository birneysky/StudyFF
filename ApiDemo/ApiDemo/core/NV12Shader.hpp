//
//  NV12Shader.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/16.
//

#ifndef NV12Shader_hpp
#define NV12Shader_hpp

#include <stdio.h>

//#include <OpenGLES/ES2/gl.h>
//#include <OpenGLES/ES2/glext.h>
#include "ShadingHelper.h"
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#include <OpenGLES/gltypes.h>
#include <string>
static const int kYTextureUnit = 0;
static const int kUvTextureUnit = 1;






class NV12Shader {
private:
    GLuint _nv12Program;
    GLint  _position;
    GLint _textureCoord;
    GLint _InputTextureUniform;
    GLuint _vao;
    GLuint _vbo;
    
    
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
        uniform sampler2D inputImageTexture;
        uniform lowp sampler2D s_textureY;
        uniform lowp sampler2D s_textureUV;
        void main() {

            mediump vec3 yuv;
        
            yuv.x = texture2D(s_textureY, v_texcoord).r;
            yuv.yz = texture2D(s_textureUV, v_texcoord).ra - vec2(0.5, 0.5);
        
            mediump mat3 matrix = mat3( 1.164,  1.164, 1.164,
                                        0.0,   -0.213, 2.112,
                                        1.793, -0.533, 0.0 );
            lowp vec3 rgb = matrix * yuv;
            gl_FragColor = vec4(rgb,1.0);
        }
    )";
private:
    bool buildProgram(const std::string& vertextShader, const std::string& fragmentShader) {
        bool result = false;
        GLuint vertShader = 0, fragShader = 0;
        _nv12Program = glCreateProgram();
        vertShader = compileShader(GL_VERTEX_SHADER, (GLchar*)vertexShader.c_str());
        //NSAssert(vertShader != 0, @"compile vertex shader failed");
        
        fragShader = compileShader(GL_FRAGMENT_SHADER, (GLchar*)fragmentShader.c_str());
        //NSAssert(fragShader != 0, @"compile fragment shader failed");
        
        glAttachShader(_nv12Program, vertShader);
        glAttachShader(_nv12Program, fragShader);
        
        glLinkProgram(_nv12Program);
        
        _position = glGetAttribLocation(_nv12Program, "position");
        _textureCoord = glGetAttribLocation(_nv12Program, "texcoord");
        
        
        GLint status;
        glGetProgramiv(_nv12Program, GL_LINK_STATUS, &status);
        //NSAssert(status == GL_TRUE, @"Failed to link program %d", _nv12Program);
        
        result = validateProgram(_nv12Program);
        //NSAssert(result, @"etup GL programm failed");
        
        GLint ySampler = glGetUniformLocation(_nv12Program, "s_textureY");
        GLint uvSampler = glGetUniformLocation(_nv12Program, "s_textureUV");

//        NSAssert(ySampler >= 0, @"Failed to get uniform variable locations in NV12 shader");
//        NSAssert(uvSampler >= 0, @"Failed to get uniform variable locations in NV12 shader");

        glUseProgram(_nv12Program);
        glUniform1i(ySampler, kYTextureUnit);
        glUniform1i(uvSampler, kUvTextureUnit);
        
        return result;
    }
    
    
public:
    NV12Shader() {
        
    }
    
    void applay(int width, int height, GLuint* textures, int len) {
        if (!_nv12Program) {
            buildProgram(vertexShader, fragmentShader);
            //[self buildProgramWithVertextShader:vertexShader fragmentShader:fragmentShader];
//            glUseProgram(_nv12Program);
//            glBindVertexArray(_vao);
        }
        
        
//        if (!_nv12Program) {
//            [self buildProgramWithVertextShader:vertexShader fragmentShader:fragmentShader];
//        }
//
        static const GLfloat imageVertices[] = {
            -1.0f, -1.0f,
            1.0f, -1.0f,
            1.0f,  1.0f,
            -1.0f,  1.0f,
        };

        GLfloat noRotationTextureCoordinates[] = {
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
        };

        glVertexAttribPointer(_position, 2, GL_FLOAT, 0, 0, imageVertices);
        glEnableVertexAttribArray(_position);
        glVertexAttribPointer(_textureCoord, 2, GL_FLOAT, 0, 0, noRotationTextureCoordinates);
        glEnableVertexAttribArray(_textureCoord);

        glUseProgram(_nv12Program);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        
        for (int i = 0; i < len; i++) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + i));
            glBindTexture(GL_TEXTURE_2D, textures[i]);
        }

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        
    }

};
#endif /* NV12Shader_hpp */
