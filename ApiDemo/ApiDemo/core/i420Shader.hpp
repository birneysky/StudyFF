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
        uniform lowp sampler2D s_textureY;
        uniform lowp sampler2D s_textureU;
        uniform lowp sampler2D s_textureV;

        void main() {
            mediump vec3 yuv;
            yuv.x = texture2D(s_textureY, v_texcoord).r;
            yuv.y = texture2D(s_textureU, v_texcoord).r - 0.5;
            yuv.z = texture2D(s_textureV, v_texcoord).r - 0.5;

            mediump mat3 matrix = mat3( 1.0,  0.0,  1.28033,
                                   1.0, -0.21482, -0.38059,
                                   1.0,  2.12798,  0.0 );


            lowp vec3 rgb = matrix * yuv;
            gl_FragColor = vec4(rgb, 1.0);
        }
    )";
    

    
//            yuv.y = yuv.y - 0.5;
//            yuv.z = yuv.z - 0.5;
        /*
         
         mediump vec3 yuv;

         yuv.x = texture2D(s_textureY, v_texcoord).r;
         yuv.y = texture2D(s_textureU, v_texcoord).r;
         yuv.z = texture2D(s_textureV, v_texcoord).r;

          mediump mat3 matrix = mat3( 1,  0, 1.28033,
                                      1,   -0.21482, -0.38509,
                                      1, 2.12798, 0.0 );
         lowp vec3 rgb = matrix * yuv;
         gl_FragColor = vec4(rgb,1.0);
         
         mediump mat3 matrix = mat3( 1,  0, 1.28033,
                                     1,   -0.21482, -0.38509,
                                     1, 2.12798, 0.0 );
         
         
         mediump mat3 matrix = mat3( 1.164,  1.164, 1.164,
                                     0.0,   -0.213, 2.112,
                                     1.793, -0.533, 0.0 );
         
         varying highp vec2 v_texcoord;
         uniform lowp sampler2D s_textureY;
         uniform lowp sampler2D s_textureU;
         uniform lowp sampler2D s_textureV;
         void main() {
             float y, u, v, r, g, b;
             y = texture2D(s_textureY, v_texcoord).r;
             u = texture2D(s_textureU, v_texcoord).r;
             v = texture2D(s_textureV, v_texcoord).r;
          
             u = u - 0.5;
             v = v - 0.5;
             r = y + 1.403 * v;
             g = y - 0.344 * u - 0.714 * v;
             b = y + 1.770 * u;
             gl_FragColor = vec4(r, g, b, 1.0);
         }
         */
    
private:
    bool buildProgram() {
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
        GLint uSampler = glGetUniformLocation(_nv12Program, "s_textureU");
        GLint vSampler = glGetUniformLocation(_nv12Program, "s_textureV");
        
        //        NSAssert(ySampler >= 0, @"Failed to get uniform variable locations in NV12 shader");
        //        NSAssert(uvSampler >= 0, @"Failed to get uniform variable locations in NV12 shader");
        
        //glUseProgram(_nv12Program);
        glUniform1i(ySampler, kYTextureUnit0);
        glUniform1i(uSampler, kUTextureUnit1);
        glUniform1i(vSampler, kVTextureUnit2);
//
        return result;
    }
    
    
public:
    I420Shader() {
        
    }
    
    void applay(int width, int height, GLuint* textures, int len) {
        if (!_nv12Program) {
            buildProgram();
        }
 
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
        

#endif /* i420Shader_hpp */