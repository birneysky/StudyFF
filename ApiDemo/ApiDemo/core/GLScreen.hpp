//
//  GLScreen.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/15.
//

#ifndef GLScreen_hpp
#define GLScreen_hpp

#include <stdio.h>

#include "GLTextureFrame.hpp"

class GLScreen : public Linkable <GLTextureFrame>{
private:
    std::function<void (GLTextureFrame* frame)> renderCallBack;
public:
    
    GLScreen(std::function<void (GLTextureFrame* frame)> render) {
        renderCallBack = render;
    }
    
    void start() {
        while (true) {
            @autoreleasepool {
                GLTextureFrame* frame = getFrame();
                if (!frame) {
                    break;
                }
                if (renderCallBack) { renderCallBack(frame); };
                [NSThread sleepForTimeInterval:0.05];
            }
        }
        std::cout << "render done" << std::endl;
    }
    
    virtual int getNumInputs() const override {
        return 1;
    }
    
    virtual int getNumOutputs() const override {
        return 0;
    }
    
    
    virtual void setOutput(int index, Linkable& filter, int port) override {
    }
    
    virtual GLTextureFrame* getFrame(int index = 0) const override {
        Link link = getInputLink(0);
        if (!link.target) {
            return nullptr;
            
        }
        return link.target->getFrame(link.port);
    }
};


#endif /* GLScreen_hpp */
