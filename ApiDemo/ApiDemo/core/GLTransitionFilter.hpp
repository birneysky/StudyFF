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
    GLTextureFrame* lastFrame0 = nullptr;
    GLTextureFrame* firstFrame1 = nullptr;
    float progress = 0.0f;
    GLint texture1Index;
    GLint texture2Index;
    GLint progreddIndex;
public:
    const std::string fragmentShader = R"(
        precision mediump float;
        varying highp vec2 textureCoordinate;
        uniform sampler2D s_Texture1;
        uniform sampler2D s_Texture2;
        uniform float progress;
        void main() {
            vec4 resColor = vec4(progress,0.0,0.0,1.0);
            float R = 1.0 - progress;
            if (textureCoordinate.x >= R) {
                resColor = texture2D(s_Texture2, vec2(textureCoordinate.x - R, textureCoordinate.y));
            } else {
                resColor = texture2D(s_Texture1, vec2(textureCoordinate.x - R + 1.0, textureCoordinate.y));
            }
            gl_FragColor = resColor;
        }
    )";
public:
    
    GLTransitionFilter(){
    }

    ~GLTransitionFilter() {
        std::cout << "~GLTransitionFilter() " << this << std::endl;
    }
    
    int getNumInputs() const override {
        return 2;
    }
    
    int getNumOutputs() const override {
        return 1;
    }
    
    const std::string& getfragmentShader() override{
        return fragmentShader;
    };
    
    
    void uploadTexture() override {
        if (progress >= 1.0f) {
            return;
        }
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, lastFrame0->getTexture());
        glUniform1i(texture1Index, 0);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, firstFrame1->getTexture());
        glUniform1i(texture2Index, 1);
        
        glUniform1f(progreddIndex, progress);

   }
     void getAttributeAndUniformIndex()  override{
         GLFilter::getAttributeAndUniformIndex();
         
         texture1Index = program->getUniformIndex("s_Texture1");
         texture2Index = program->getUniformIndex("s_Texture2");
         progreddIndex = program->getUniformIndex("progress");
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
           progress += 0.02;
           GLTextureFrame* outFrame = getOutput(firstFrame1->getWidth(), firstFrame1->getHeight());
           return outFrame;

       } else {
           lastFrame0 = frame1;
           return frame1;
       }
       
    }
};


#endif /* GLTransitionFilter_hpp */
