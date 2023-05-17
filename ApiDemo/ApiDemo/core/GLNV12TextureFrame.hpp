//
//  GLNV12TextureFrame.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/15.
//

#ifndef GLNV12TextureFrame_hpp
#define GLNV12TextureFrame_hpp

#include "GLTextureFrame.hpp"

class GLNV12TextureFrame: public GLTextureFrame {
private:
    GLuint _uvTexture;
public:
    GLNV12TextureFrame(AVFrame* frame) {
        int width = frame->width;
        int height = frame->height;
        GLTextureFrame(width, height);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame->data[0]);
        glActiveTexture(GL_TEXTURE1);
        glGenTextures(1, &_uvTexture);
        glBindTexture(GL_TEXTURE_2D, _uvTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // This is necessary for non-power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, _width / 2, _height / 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame->data[1]);
        
    }
    
    
}

#endif /* GLNV12TextureFrame_hpp */
