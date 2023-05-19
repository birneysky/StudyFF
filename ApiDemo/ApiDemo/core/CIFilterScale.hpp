//
//  ScaleFilter.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/10.
//

#ifndef ScaleFilter_hpp
#define ScaleFilter_hpp

#include "Filter.hpp"

class CIFilterScale: public Filter<CIImage> {

public:
    CIFilterScale():Filter("scale") {
        
    }
    
    virtual void setOutput(int index, Linkable& filter, int port) override {
        
    }
    
    virtual CIImage* getFrame(int index = 0) override {
        Link link = getInputLink(0);
        if (!link.target) {
            return nullptr;
            
        }
        
        
        CIImage* image = link.target->getFrame(link.port);

        // 创建CILanczosScaleTransform滤镜
        CIFilter *scaleFilter = [CIFilter filterWithName:@"CILanczosScaleTransform"];
        [scaleFilter setValue:image forKey:kCIInputImageKey];
        [scaleFilter setValue:@(0.4) forKey:kCIInputScaleKey];
        [scaleFilter setValue:@(1.0) forKey:kCIInputAspectRatioKey];
        
        // 获取输出图像
        CIImage *outputImage = [scaleFilter outputImage];
       
        
        return outputImage;
    }
    
    
};

#endif /* ScaleFilter_hpp */
