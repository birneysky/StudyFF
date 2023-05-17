//
//  TextureRender.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/16.
//

#ifndef TextureRender_hpp
#define TextureRender_hpp

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#include <OpenGLES/gltypes.h>
#include <string>
#include <CoreVideo/CoreVideo.h>
#include "NV12Shader.hpp"
#include "NV12TextCache.hpp"
#include "I420TextureCache.hpp"
#include "i420Shader.hpp"

class TextureRender1 {
private:
    EAGLContext* _glContext = nullptr;
    NV12Shader* shader1 = nullptr;
    NV12TextCache* textCache = nullptr;
    I420Shader* i420shader = nullptr;
    I420TextureCache* i420Cache = nullptr;
    GLuint _targetTexture = 0;
    GLuint _targetFBO = 0;
    GLTextureFrame* textFrame = nullptr;
private:
    void setupContextAndBuffers(int width, int height) {
        bool success = [EAGLContext setCurrentContext:_glContext];
        //NSAssert(success == YES, @"EAGLContext setCurrentContext failed");
        
        //NSAssert(_glContext, @"EAGLContext alloc failed");
        if(!textFrame) {
            textFrame = new GLTextureFrame(width, height);
        }
        
        glGenFramebuffers(1, &_targetFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, _targetFBO);
        
//        glGenTextures(1, &_targetTexture);
//        glBindTexture(GL_TEXTURE_2D, _targetTexture);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        //glBindTexture(GL_TEXTURE_2D, 0); // unbind
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textFrame->getTexture(), 0);
        
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        //NSAssert(status == GL_FRAMEBUFFER_COMPLETE, @"failed to make complete framebuffer object %x",status);
        
        
        GLenum glError = glGetError();
        //NSAssert(glError == GL_NO_ERROR, @"failed to setup GL %x",glError);
        
    }
public:
    TextureRender1(EAGLContext * context) {
        _glContext = context;
    }
    
    void render(AVFrame* frame) {
        if (!frame) {
            return;
        }
        int width = frame->width;
        int height = frame->height;
        
        if ([EAGLContext currentContext] != _glContext) {
           NSLog(@"EAGLContext currentContext is not _glContext");
            [EAGLContext setCurrentContext:_glContext];
        }
        
        if (!i420shader) {
            setupContextAndBuffers((int)width, (int)height);
            i420shader = new I420Shader();
        }
        
        
        if (!i420Cache) {
            i420Cache = new I420TextureCache(width, height);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, _targetFBO);
        i420Cache->uploadTexturesData(frame);
        i420shader->applay((int)width, (int)height, i420Cache->textureIds(), i420Cache->getTextureCount());
    }
    
    void render(void* buffer) {
        CVPixelBufferRef pixelBuffer = (CVPixelBufferRef)buffer;
        OSType pixelFmt = CVPixelBufferGetPixelFormatType(pixelBuffer);
        assert(pixelFmt == kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange);
        
        size_t width  = CVPixelBufferGetWidth(pixelBuffer);
        size_t height = CVPixelBufferGetHeight(pixelBuffer);
        
        if (!shader1) {
            setupContextAndBuffers((int)width, (int)height);
            shader1 = new NV12Shader();
        }
        
        if ([EAGLContext currentContext] != _glContext) {
           NSLog(@"EAGLContext currentContext is not _glContext");
            [EAGLContext setCurrentContext:_glContext];
        }

        if (!textCache) {
            textCache = new NV12TextCache(_glContext);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, _targetFBO);
        //glViewport(0, 0, width, height);
        // 渲染 FBO 2
        textCache->uploadTexturesData(buffer);
        shader1->applay((int)width, (int)height, textCache->textureIds(), textCache->getTextureCount());
    }
    
    GLTextureFrame* outputTexture() {
        return textFrame;
        //return _targetTexture;
    }
    
    
};

#endif /* TextureRender_hpp */
