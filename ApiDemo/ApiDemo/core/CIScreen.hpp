//
//  Screen.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/10.
//

#ifndef Screen_hpp
#define Screen_hpp

#include <stdio.h>
#include <iostream>

class CIScreen : public Linkable <CIImage>{
private:    
    std::function<void (CIImage* frame)> renderCallBack;
public:
    
    CIScreen(std::function<void (CIImage* frame)> render) {
        renderCallBack = render;
    }
    
    void start() {
        while (true) {
            @autoreleasepool {
                CIImage* frame = getFrame();
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
    
    virtual CIImage* getFrame(int index = 0) const override {
        Link link = getInputLink(0);
        if (!link.target) {
            return nullptr;
            
        }
        return link.target->getFrame(link.port);
    }
};

#endif /* Screen_hpp */
