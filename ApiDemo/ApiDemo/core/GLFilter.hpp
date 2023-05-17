//
//  GLFilter.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/12.
//

#ifndef GLFilter_hpp
#define GLFilter_hpp

#include "GLProgram.hpp"
#include "GLTextureFrame.hpp"
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import <OpenGLES/gltypes.h>

class GLFilter {
private:
    const std::string vertexShader = R"(
             attribute vec4 position;
             attribute vec4 texcoord;
             
             varying vec2 textureCoordinate;
             
             void main()
             {
                 gl_Position = position;
                 textureCoordinate = texcoord.xy;
             }
    )";
    
    
    const std::string fragmentShader = R"(
        varying highp vec2 textureCoordinate;
        uniform sampler2D inputImageTexture;
     
        void main()
        {
            gl_FragColor = texture2D(inputImageTexture, textureCoordinate);
        }
    )";

private:
    static GLContext context;
    GLProgram* program = nullptr;
    GLTextureFrame* inputTexure = nullptr;
    GLuint _frameBuffer;
//    GLuint _renderBuffer;
    GLint _Width;
    GLint _Height;
    GLint  _position;
    GLint _textureCoord;
    GLuint _vao;
    GLuint _vbo;
     
protected:
    virtual void process(GLTextureFrame* frame) {
        
    }
    
public:
    GLFilter() {
        program =  new GLProgram(vertexShader, fragmentShader);
        if (!program->link()) {
            abort();
        }
//        glGenRenderbuffers(1, &_renderBuffer);
        _position = program->getAttributeIndex("position");
        _textureCoord = program->getAttributeIndex("texcoord");
        
    }
    
    const std::string& getVertexShader() {
        return vertexShader;
    }
    
    virtual const std::string getfragmentShader() {
        return fragmentShader;
    };
    
    void setInput(GLTextureFrame* frame) {
        inputTexure = frame;
    }
    
    GLTextureFrame* getOutput() {
        if (inputTexure) {
            /// set context
            context.useCurrentContext();
            /// set buffers
            
            glGenBuffers(1, &_vbo);
            glGenVertexArrays(1, &_vao);
            /// 左侧顶点坐标，右侧纹理坐标
            static const GLfloat vertices[] = {
                -1.0f, -1.0f,  0.0f, 1.0f,
                 1.0f, -1.0f,  1.0f, 1.0f,
                 1.0f,  1.0f,  1.0f, 0.0f,
                -1.0f,  1.0f,  0.0f, 0.0f,
            };
            glBindVertexArray(_vao);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            
            // 3. 设置顶点属性指针
            glVertexAttribPointer(_position, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
            glEnableVertexAttribArray(_position);
            
            glVertexAttribPointer(_textureCoord, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
            glEnableVertexAttribArray(_textureCoord);
            
//            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderBuffer);
//            glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
            /// upload texture
            glBindTexture(GL_TEXTURE_2D, inputTexure->getTexture());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            /// using gramm
            process(inputTexure);
            program->use();
            /// draw
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }
        
        return inputTexure;
    }
    
    ~GLFilter() {
        if (program) {
            delete program;
            program = nullptr;
        }
    }
    
    
    
};

#endif /* GLFilter_hpp */
