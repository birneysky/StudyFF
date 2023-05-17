//
//  GLTextureFrame.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/12.
//

#ifndef TextureFrame_hpp
#define TextureFrame_hpp

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

class GLTextureFrame {
protected:
    GLuint _framebuffer;
    GLuint _texture = 0;
    int _width;
    int _height;
private:
    void createTexture() {
        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, _texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);
    }
    
    void createFramebuffer() {
//        glGenFramebuffers(1, &_framebuffer);
//        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
        
    }
    
public:
    
    GLTextureFrame(int width, int height):_width(width), _height(height) {
        createTexture();
        createFramebuffer();
    }
    
    ~GLTextureFrame() {
        
    }
        
    void activeFrameBuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
        glViewport(0, 0, (int)_width, (int)_height);
    }
    
    int getWidth() const {
        return _width;
    }
    
    int getHeight() const {
        return _width;
    }
    
    GLuint getTexture() const {
        return _texture;
    }
};

#endif /* TextureFrame_hpp */
