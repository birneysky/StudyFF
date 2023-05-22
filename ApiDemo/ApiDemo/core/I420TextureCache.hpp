//
//  I420TextureCache.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/16.
//

#ifndef I420TextureCache_hpp
#define I420TextureCache_hpp

#include <stdio.h>
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#include <OpenGLES/gltypes.h>

class I420TextureCache {
private:
//    GLuint _ytexture = 0;
//    GLuint _utexture = 0;
//    GLuint _vtexture = 0;
    GLuint* _texutres = nullptr;
    const int textureCount = 3;
    uint8_t* yBuffer = nullptr;
    uint8_t* uBuffer = nullptr;
    uint8_t* vBuffer = nullptr;

public:
    
    I420TextureCache(int width, int height) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        _texutres = new GLuint[textureCount];
        glGenTextures(3, _texutres);
        for (int i = 0; i < 3; i++) {
            glBindTexture(GL_TEXTURE_2D, _texutres[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            //
            //glBindTexture(GL_TEXTURE_2D, 0);
            yBuffer = new uint8_t[width*height] {};
            uBuffer = new uint8_t[width*height/4] {};
            vBuffer = new uint8_t[width*height/4] {};
        }
        
    }
    
    bool uploadTexturesData(AVFrame* frame) {
        if (!frame) {
            return false;
        }
        int width = frame->width;
        int height = frame->height;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _texutres[0]);
        
        uint8_t* dptr = yBuffer;
        uint8_t* srcPtr = frame->data[0];
        for(int i = 0; i < height; i++) {
            memcpy(dptr, srcPtr, width);
            dptr += width;
            srcPtr += frame->linesize[0];
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, yBuffer);
        

        dptr = uBuffer;
        srcPtr = frame->data[1];
        for(int i = 0; i < height / 2; i++) {
            memcpy(dptr, srcPtr, width / 2);
            dptr += width / 2;
            srcPtr += frame->linesize[1];
        }
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _texutres[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width / 2, height / 2, 0, GL_LUMINANCE, GL_LUMINANCE, uBuffer);

        
        dptr = vBuffer;
        srcPtr = frame->data[2];
        for(int i = 0; i < height / 2; i++) {
            memcpy(dptr, srcPtr, width / 2);
            dptr += width / 2;
            srcPtr += frame->linesize[2];
        }
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, _texutres[2]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width / 2, height / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, vBuffer);

//        for (int i = 1; i < textureCount; i++) {
//            glActiveTexture(GL_TEXTURE0 + i);
//            glBindTexture(GL_TEXTURE_2D, _texutres[i]);
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width / 2, height / 2, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, frame->data[i]);
//        }
        return true;
    }
    
    GLuint* textureIds () {
        return _texutres;
    }

    int getTextureCount() {
        return textureCount;
    }
    
    
};

#endif /* I420TextureCache_hpp */
