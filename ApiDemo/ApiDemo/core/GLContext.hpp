//
//  GLContext.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/12.
//

#ifndef GLContext_hpp
#define GLContext_hpp
#include <OpenGLES/ES2/gl.h>

class GLContext {
private:
    EAGLContext* _context;
public:
    
    GLContext() {
        _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    }
    
    void useCurrentContext() {
        if ([EAGLContext currentContext] != _context) {
            [EAGLContext setCurrentContext:_context];
        }
        
    }
};

#endif /* GLContext_hpp */
