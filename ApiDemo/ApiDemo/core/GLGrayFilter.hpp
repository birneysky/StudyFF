//
//  GLGrayFilter.hpp
//  ApiDemo
//
//  Created by Bruce on 2023/5/19.
//  Copyright © 2023 Grocery. All rights reserved.
//

#ifndef GLGrayFilter_hpp
#define GLGrayFilter_hpp

#include "Filter.hpp"
#include "GLTextureFrame.hpp"
#include "GLFilter.hpp"

class GLGrayFilter: public GLFilter {
public:
    const std::string fragmentShader = R"(
        varying highp vec2 textureCoordinate;
        uniform sampler2D inputImageTexture;
        const mediump vec3 LUMINANCE_FACTOR = vec3(0.2125, 0.7154, 0.0721);
        void main()
        {
            highp vec4 sampleColor = texture2D(inputImageTexture, textureCoordinate);
            lowp float luminance = dot(sampleColor.rgb, LUMINANCE_FACTOR);
            gl_FragColor = vec4(mix(vec3(luminance), sampleColor.rgb, 1.0 - 0.9), 1.0);
        }
    )";
public:
    
    GLGrayFilter(){
    }
    
    virtual GLProgram* getProgram() override {
        return new GLProgram(vertexShader, fragmentShader);;
    }
    
    
    virtual const std::string& getfragmentShader() {
        return fragmentShader;
    };
    
   virtual GLTextureFrame* getFrame(int port) override {
        Link link = getInputLink(0);
        if (!link.target) {
            return nullptr;
            
        }
        
        GLTextureFrame* frame = link.target->getFrame(link.port);
       
       //GLFilter glFilter;
       setInput(frame);
       GLTextureFrame* outFrame = getOutput();
//        CIFilter *scaleFilter = [CIFilter filterWithName:@"CIColorControls"];
//        [scaleFilter setValue:image forKey:kCIInputImageKey];
//        [scaleFilter setValue:@(0.0) forKey:kCIInputSaturationKey];
//
        // 获取输出图像
//        CIImage *outputImage = [scaleFilter outputImage];
       
        
        return outFrame;
    }
};



#endif /* GLGrayFilter_hpp */
