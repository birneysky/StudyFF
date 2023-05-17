//
//  NV12TextCache.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/16.
//

#ifndef NV12TextCache_hpp
#define NV12TextCache_hpp

#include <stdio.h>

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#include <OpenGLES/gltypes.h>
#include <string>
#include <CoreVideo/CoreVideo.h>

class NV12TextCache {
private:
    CVOpenGLESTextureCacheRef _textureCache = nullptr;
    CVOpenGLESTextureRef _yTextureRef = nullptr;
    CVOpenGLESTextureRef _uvTextureRef = nullptr;
    GLuint _textureIds[2] = {};
private:
    bool loadTexture(CVOpenGLESTextureRef *textureOut, CVPixelBufferRef pixelBuffer, int planeIndex, GLenum pixelFormat) {
        const size_t width = CVPixelBufferGetWidthOfPlane(pixelBuffer, planeIndex);
        const size_t height = CVPixelBufferGetHeightOfPlane(pixelBuffer, planeIndex);

        if (*textureOut) {
          CFRelease(*textureOut);
          *textureOut = nullptr;
        }
        CVReturn ret = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault,
                          _textureCache, pixelBuffer, NULL, GL_TEXTURE_2D, pixelFormat,
                          (GLsizei)width, (GLsizei)height, pixelFormat, GL_UNSIGNED_BYTE, planeIndex, textureOut);
        if (ret != kCVReturnSuccess) {
          CFRelease(*textureOut);
          *textureOut = nullptr;
          return false;
        }

//        NSAssert(CVOpenGLESTextureGetTarget(*textureOut) == GL_TEXTURE_2D,
//                 @"Unexpected GLES texture target");
        glBindTexture(GL_TEXTURE_2D, CVOpenGLESTextureGetName(*textureOut));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        return true;
    }
    
    GLuint yTexture() {
      return CVOpenGLESTextureGetName(_yTextureRef);
    }

    GLuint uvTexture() {
      return CVOpenGLESTextureGetName(_uvTextureRef);
    }
        
public:
    NV12TextCache(EAGLContext * context) {
        CVReturn ret = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault,
                                                    NULL,
                                                    context,
                                                    NULL,
                                                    &_textureCache);
        assert(ret == kCVReturnSuccess);
    }
    
    ~NV12TextCache() {
        if (_uvTextureRef) {
          CFRelease(_uvTextureRef);
          _uvTextureRef = nullptr;
        }
        if (_yTextureRef) {
          CFRelease(_yTextureRef);
          _yTextureRef = nullptr;
        }
        
        if (_textureCache) {
          CFRelease(_textureCache);
          _textureCache = nullptr;
        }
    }
    
    
    GLuint* textureIds () {
        _textureIds[0] = yTexture();
        _textureIds[1] = uvTexture();
        return _textureIds;
    }

    int getTextureCount() {
        return 2;
    }

    bool uploadTexturesData(void* buffer) {
        CVPixelBufferRef pixelBuffer = (CVPixelBufferRef)buffer;
        OSType pixelFmt = CVPixelBufferGetPixelFormatType(pixelBuffer);
        assert(pixelFmt == kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange);
        return loadTexture(&_yTextureRef, pixelBuffer, 0, GL_LUMINANCE) &&
        loadTexture(&_uvTextureRef, pixelBuffer, 1, GL_LUMINANCE_ALPHA);
    }
};

#endif /* NV12TextCache_hpp */
