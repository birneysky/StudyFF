//
//  GLOverlayFilter.hpp
//  ApiDemo
//
//  Created by Bruce on 2023/6/2.
//  Copyright © 2023 Grocery. All rights reserved.
//

#ifndef GLOverlayFilter_hpp
#define GLOverlayFilter_hpp


#include "Filter.hpp"
#include "GLTextureFrame.hpp"
#include "GLFilter.hpp"

class GLOverlayFilter: public GLFilter {
private:
    GLTextureFrame* overlayFrame = nullptr;
    GLTextureFrame* backgroundFrame = nullptr;
    GLint overlayTextureIndex;
    GLint backgroundTextureIndex;
    GLint lbPositionIndex;
    GLint rtPositionIndex;
public:
    const std::string fragmentShader = R"(
         varying highp vec2 textureCoordinate;
         varying lowp vec2 v_postion;
         
         uniform lowp vec2 lb_position;
         uniform lowp vec2 rt_position;

         uniform sampler2D s_overlayTexture;
         uniform sampler2D s_backgroundTexture;
         void main() {
            mediump vec3 rgb = texture2D(s_backgroundTexture, textureCoordinate).rgb;
            
            if (v_postion.x >= lb_position.x && v_postion.y >= lb_position.y &&
                v_postion.x <= rt_position.x && v_postion.y <= rt_position.y) {
                
                lowp vec2 pos = vec2((v_postion.x - lb_position.x) / (rt_position.x - lb_position.x),
                                    (v_postion.y - lb_position.y) / (rt_position.y - lb_position.y));
                lowp vec4 overlayColor = texture2D(s_overlayTexture, pos);
                lowp float r = overlayColor.r + (1.0 - overlayColor.a)*rgb.r;
                lowp float g = overlayColor.g + (1.0 - overlayColor.a)*rgb.g;
                lowp float b = overlayColor.b + (1.0 - overlayColor.a)*rgb.b;
                lowp float a = 1.0;
                gl_FragColor = vec4(r, g, b, a);
            } else {
                gl_FragColor = vec4(rgb,1.0);
            }
         }
    )";
public:
    
    GLOverlayFilter(){
    }

    ~GLOverlayFilter() {
        std::cout << "~GLOverlayFilter() " << this << std::endl;
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
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, overlayFrame->getTexture());
        glUniform1i(overlayTextureIndex, 0);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, backgroundFrame->getTexture());
        glUniform1i(backgroundTextureIndex, 1);
        
        //glUniform1f(progreddIndex, progress);

        glUniform2f(lbPositionIndex, 0.0, 0.0);
        glUniform2f(rtPositionIndex, 0.4, 0.3);
   }
     void getAttributeAndUniformIndex() override {
         GLFilter::getAttributeAndUniformIndex();
         
         overlayTextureIndex = program->getUniformIndex("s_overlayTexture");
         backgroundTextureIndex = program->getUniformIndex("s_backgroundTexture");
         lbPositionIndex = program->getUniformIndex("lb_position");
         rtPositionIndex = program->getUniformIndex("rt_position");
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
       
       GLTextureFrame* background = link0.target->getFrame(link0.port);
       if (background) {
           backgroundFrame = background;
       }
       
       
       GLTextureFrame* overlay = link1.target->getFrame(link1.port);
       overlayFrame = overlay;
       
       if (!overlay) {
           return background;
       }
       
       
       
       GLTextureFrame* outFrame = getOutput(backgroundFrame->getWidth(), backgroundFrame->getHeight());
       return outFrame;
    }
};


#endif /* GLOverlayFilter_hpp */
