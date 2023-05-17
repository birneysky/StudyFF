//
//  OverlayFilter.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/11.
//

#ifndef OverlayFilter_hpp
#define OverlayFilter_hpp

#include "Filter.hpp"

class CIFilterOverlay: public Filter<CIImage> {

public:
    CIFilterOverlay():Filter("Gray") {
        
    }
    
    virtual int getNumInputs() const override {
        return 2;
    }

    
    virtual void setOutput(int index, Linkable<CIImage>& filter, int port) override {
        
    }
    
    virtual CIImage* getFrame(int index = 0) const  override {
        Link link0 = getInputLink(0);
        if (!link0.target) {
            abort();
        }
        
        
        Link link1 = getInputLink(1);
        if (!link1.target) {
            abort();
        }
        
        CIImage* backgroundImage = link0.target->getFrame(link0.port);
        CIImage* foregroundImage = link1.target->getFrame(link1.port);
        

        CIFilter *transformFilter = [CIFilter filterWithName:@"CIAffineTransform"];
        [transformFilter setValue:foregroundImage forKey:kCIInputImageKey];

        CGAffineTransform transform = CGAffineTransformMakeTranslation(10, 300);
        [transformFilter setValue:[NSValue valueWithCGAffineTransform:transform] forKey:kCIInputTransformKey];

    
        CIFilter *compositingFilter = [CIFilter filterWithName:@"CISourceOverCompositing"];
        [compositingFilter setValue:backgroundImage forKey:kCIInputBackgroundImageKey];
        [compositingFilter setValue:transformFilter.outputImage forKey:kCIInputImageKey];

        // 获取输出图像
        CIImage *outputImage = [compositingFilter outputImage];
  
        return outputImage;
    }
    
    
};

#endif /* OverlayFilter_hpp */
