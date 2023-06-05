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

class GLPlayer : public Linkable <GLTextureFrame>{
private:
    std::function<void (GLTextureFrame* frame)> renderCallBack;
    enum class Status {unknown, playing, pause, stop};
    
    Status _status;
public:
    
    GLPlayer(std::function<void (GLTextureFrame* frame)> render) {
        renderCallBack = render;
    }
    
    ~GLPlayer() {
        std::cout << "~GLScreen() " << this << std::endl;
    }
    
    void start() {
        _status = Status::playing;
        while (true) {
            @autoreleasepool {
                if (_status == Status::stop) {
                    break;
                }
                if (_status != Status::playing) {
                    [NSThread sleepForTimeInterval:0.05];
                    continue;
                }
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
    
    void pause() {
        _status = Status::pause;
    }
    
    void resume() {
        _status = Status::playing;
    }
    
    int getNumInputs() const override {
        return 1;
    }
    
    int getNumOutputs() const override {
        return 0;
    }
    
    
    void setOutput(int index, Linkable& filter, int port) override {
    }
    
    GLTextureFrame* getFrame(int index = 0) override {
        Link link = getInputLink(0);
        if (!link.target) {
            return nullptr;
            
        }
        return link.target->getFrame(link.port);
    }
};


#endif /* GLScreen_hpp */
