//
//  GLTransitionFilter.hpp
//  ApiDemo
//
//  Created by Bruce on 2023/5/22.
//  Copyright © 2023 Grocery. All rights reserved.
//

#ifndef GLTransitionFilter_hpp
#define GLTransitionFilter_hpp

#include "Filter.hpp"
#include "GLTextureFrame.hpp"
#include "GLFilter.hpp"

class GLTransitionFilter: public GLFilter {
private:
    GLTextureFrame* lastFrame0;
    GLTextureFrame* firstFrame1;
public:
    const std::string fragmentShader = R"(
        varying highp vec2 textureCoordinate;
        uniform sampler2D s_Texture1;
        uniform sampler2D s_Texture2;
        uniform float progress;
        void main()
        {
            vec2 p = textureCoordinate.xy/vec2(1.0).xy;
            highp vec4 sample1 = texture2D(s_Texture1,p);
            highp vec4 sample2 = texture2D(s_Texture2,p);
            gl_FragColor = mix(sample1, sample2, step(1.0-p.x,progress));
        }
    )";
public:
    
    GLTransitionFilter(){
    }
    
    int getNumInputs() const override {
        return 2;
    }
    
    int getNumOutputs() const override {
        return 1;
    }
    
    GLProgram* getProgram() override {
        if (!program) {
            program = new GLProgram(vertexShader, fragmentShader);
        }
        return program;
    }
    
    
    const std::string& getfragmentShader() override{
        return fragmentShader;
    };
    
    
    void uploadTexture() override {
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, getInput()->getTexture());
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
       if (!frame) {
           
           GLTextureFrame* frame2 = link1.target->getFrame(link1.port);
           //lastFrame0
           /// to do image process
           ///
           GLTextureFrame* outFrame = getOutput();
           return outFrame;
       } else {
           lastFrame0 = frame1;
           return frame1;
       }
       
//       setInput(frame);
        
    }
};


#endif /* GLTransitionFilter_hpp */
