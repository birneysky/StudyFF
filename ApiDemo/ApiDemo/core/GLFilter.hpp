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
       // const mediump vec3 LUMINANCE_FACTOR = vec3(0.2125, 0.7154, 0.0721);
        void main()
        {
//            highp vec4 sampleColor = texture2D(inputImageTexture, textureCoordinate);
//            lowp float luminance = dot(sampleColor.rgb, LUMINANCE_FACTOR);
//            gl_FragColor = vec4(mix(vec3(luminance), sampleColor.rgb, 1.0 - 0.9), 1.0);
            gl_FragColor = texture2D(inputImageTexture, textureCoordinate);
        }
    )";
    
private:
    //static GLContext context;
    GLProgram* program = nullptr;
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
            program = new GLProgram(vertexShader, fragmentShader);
        }
        return program;
    }
    
    void setInput(GLTextureFrame* frame) {
        inputTexure = frame;
    }
    
    GLTextureFrame* getOutput() {
        if (inputTexure) {
            if (!program) {
                program = getProgram();
                if (!program->link()) {
                    abort();
                }
        //        glGenRenderbuffers(1, &_renderBuffer);
                _position = program->getAttributeIndex("position");
                _textureCoord = program->getAttributeIndex("texcoord");
                _inputTextureIndex = program->getUniformIndex("inputImageTexture");
            }
            
            if (!output) {
                output = new GLTextureFrame(inputTexure->getWidth(), inputTexure->getHeight());
                glGenFramebuffers(1, &_targetFBO);
                glBindFramebuffer(GL_FRAMEBUFFER, _targetFBO);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, output->getTexture(), 0);
            }
            
            
            glBindFramebuffer(GL_FRAMEBUFFER, _targetFBO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, inputTexure->getTexture());
            
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
            
            program->use();
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            return output;
        }
        
        return inputTexure;
    }
    
    virtual GLTextureFrame* getFrame(int port) override {
        Link link = getInputLink(0);
        if (!link.target) {
            return nullptr;
        }
        
        GLTextureFrame* frame = link.target->getFrame(link.port);
        setInput(frame);
        GLTextureFrame* outFrame = getOutput();
        return outFrame;
    }
};



#endif /* GLFilter_hpp */
