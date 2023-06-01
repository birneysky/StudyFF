//
//  GLJoinFilter.hpp
//  ApiDemo
//
//  Created by Bruce on 2023/6/1.
//  Copyright © 2023 Grocery. All rights reserved.
//

#ifndef GLJoinFilter_hpp
#define GLJoinFilter_hpp

#include "Filter.hpp"
#include "GLTextureFrame.hpp"
#include "GLFilter.hpp"

class GLJoinFilter: public GLFilter {
private:
    GLTextureFrame* lastFrame0;
    GLTextureFrame* firstFrame1;

public:
    
    GLJoinFilter(){
    }
    
    int getNumInputs() const override {
        return 2;
    }
    
    int getNumOutputs() const override {
        return 1;
    }
    
   GLTextureFrame* getFrame(int port) override {
        Link link0 = getInputLink(0);
        if (!link0.target) {
            assert(false);
            return nullptr;
            
        }
       
       Link link1 = getInputLink(1);
       if (!link1.target) {
           assert(false);
           return nullptr;
           
       }
       
        GLTextureFrame* frame1 = link0.target->getFrame(link0.port);
       if (!frame1) {
           
           GLTextureFrame* frame2 = link1.target->getFrame(link1.port);
           return frame2;
       }
       return frame1;
        
    }
};



#endif /* GLJoinFilter_hpp */
