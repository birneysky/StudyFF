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
#import "Filter.hpp"
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import <OpenGLES/gltypes.h>

class GLFilter : public Filter<GLTextureFrame>{
private:
    const std::string fragmentShader = R"(
        varying highp vec2 textureCoordinate;
        uniform sampler2D inputImageTexture;
        void main()
        {
            gl_FragColor = texture2D(inputImageTexture, textureCoordinate);
        }
    )";
    
private:
    //static GLContext context;
    GLTextureFrame* inputTexure = nullptr;
    GLuint _frameBuffer;
//    GLuint _renderBuffer;
    GLint _Width;
    GLint _Height;
    GLint  _position;
    GLint _textureCoord;
    GLuint _inputTextureIndex;
    GLuint _vao;
    GLuint _vbo;
    
    GLuint _targetTexture;
    GLuint _targetFBO;
    GLTextureFrame* output = nullptr;
protected:
    GLProgram* program = nullptr;
public:
    static const std::string vertexShader;
    GLFilter() {
        
        //GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    }
    
    virtual ~GLFilter() {
        if (output) {
            delete output;
            output = nullptr;
        }
        
        if (program) {
            delete  program;
            program = nullptr;
        }
    }
    
    const std::string& getVertexShader() {
        return vertexShader;
    }
    
    virtual const std::string& getfragmentShader() {
        return fragmentShader;
    };
    
    virtual GLProgram* getProgram() {
        if (!program) {
            program = new GLProgram(getVertexShader(), getfragmentShader());
        }
        return program;
    }
    
    virtual void uploadTexture() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, getInput()->getTexture());
    }
    
    virtual void getAttributeAndUniformIndex() {
        _position = program->getAttributeIndex("position");
        _textureCoord = program->getAttributeIndex("texcoord");
        _inputTextureIndex = program->getUniformIndex("inputImageTexture");
    }
    
    GLTextureFrame* getInput() {
        return inputTexure;
    }
    void setInput(GLTextureFrame* frame) {
        inputTexure = frame;
    }
    
    GLTextureFrame* getOutput(const int& width, const int& height) {
        //if (inputTexure) {
            if (!program) {
                program = getProgram();
                if (!program->link()) {
                    abort();
                }
        //        glGenRenderbuffers(1, &_renderBuffer);
                getAttributeAndUniformIndex();
            }
            
            if (!output) {
                output = new GLTextureFrame(width, height);
                glGenFramebuffers(1, &_targetFBO);
                glBindFramebuffer(GL_FRAMEBUFFER, _targetFBO);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, output->getTexture(), 0);
            }
            
            
            program->use();
            glBindFramebuffer(GL_FRAMEBUFFER, _targetFBO);
            
            uploadTexture();
            
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
        
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            
            glVertexAttribPointer(_position, 2, GL_FLOAT, 0, 0, imageVertices);
            glEnableVertexAttribArray(_position);
            glVertexAttribPointer(_textureCoord, 2, GL_FLOAT, 0, 0, noRotationTextureCoordinates);
            glEnableVertexAttribArray(_textureCoord);
            
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glUseProgram(0);
            return output;
        //}
        
        //return inputTexure;
    }
    
    virtual GLTextureFrame* getFrame(int port) override {
        Link link = getInputLink(0);
        if (!link.target) {
            return nullptr;
        }
        
        GLTextureFrame* frame = link.target->getFrame(link.port);
        setInput(frame);
        GLTextureFrame* outFrame = getOutput(frame->getWidth(), frame->getHeight());
        return outFrame;
    }
};

#endif /* GLFilter_hpp */
