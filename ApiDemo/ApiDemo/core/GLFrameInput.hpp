//
//  GLFrameInput.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/12.
//

#ifndef GLFrameInput_hpp
#define GLFrameInput_hpp

#include "GLTextureFrame.hpp"

class GLFrameInput {
public:
    void setInputFrame:(GLTextureFrame* frame) = 0;
};


class GLFrameOutput {
public:
    void GLTextureFrame* getOutputFrame:() = 0;
    void connect(GLFrameInput& GLFrameInput) = 0;
    
};

#endif /* GLFrameInput_hpp */
