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
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            //
            //glBindTexture(GL_TEXTURE_2D, 0);
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, frame->data[0]);
        
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, _texutres[0]);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width / 2, height / 2, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, frame->data[0]);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _texutres[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width / 2, height / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, frame->data[1]);
        
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, _texutres[2]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width / 2, height / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, frame->data[2]);
        
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
