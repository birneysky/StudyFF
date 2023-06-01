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
    float progress = 0.0f;
    GLint texture1Index;
    GLint texture2Index;
    GLint progreddIndex;
public:
    const std::string fragmentShader = R"(
        varying highp vec2 textureCoordinate;
        uniform sampler2D s_Texture1;
        uniform sampler2D s_Texture2;
        uniform highp float progress;
        void main()
        {
            highp vec2 p = textureCoordinate.xy/vec2(1.0).xy;
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
        progress += 0.25;
        glUniform1f(progreddIndex, progress);
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, lastFrame0->getTexture());
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, firstFrame1->getTexture());
   }
     void getAttributeAndUniformIndex()  override{
         GLFilter::getAttributeAndUniformIndex();
         
         texture1Index = program->getUniformIndex("s_Texture1");
         texture2Index = program->getUniformIndex("s_Texture2");
         progreddIndex = program->getUniformIndex("progress");
         
         glUniform1i(texture1Index, 0);
         glUniform1i(texture2Index, 1);
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
           
           if (progress > 1.0f) {
               GLTextureFrame* frame2 = link1.target->getFrame(link1.port);
               return frame2;
           }
           
           if (!firstFrame1) {
               GLTextureFrame* frame2 = link1.target->getFrame(link1.port);
               firstFrame1 = frame2;
           }
           GLTextureFrame* outFrame = getOutput(lastFrame0->getWidth(), lastFrame0->getHeight());
           return outFrame;

       }
       
       lastFrame0 = frame1;
       return frame1;
    }
};


#endif /* GLTransitionFilter_hpp */
