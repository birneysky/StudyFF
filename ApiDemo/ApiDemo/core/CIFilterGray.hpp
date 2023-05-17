//
//  GrayFilter.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/11.
//

#ifndef GrayFilter_hpp
#define GrayFilter_hpp


#include "Filter.hpp"

class CIFilterGray: public Filter<CIImage> {

public:
    CIFilterGray():Filter("Gray") {
        
    }

//    virtual void setInput(int index, Linkable& filter, int port) override {
//        this->input = &filter;
//        this->inputPort = port;
//    }
    
    virtual void setOutput(int index, Linkable& filter, int port) override {
        
    }
    
    virtual CIImage* getFrame(int index = 0) const  override {
        Link link = getInputLink(0);
        if (!link.target) {
            return nullptr;
            
        }
        
        CIImage* image = link.target->getFrame(link.port);
        CIFilter *scaleFilter = [CIFilter filterWithName:@"CIColorControls"];
        [scaleFilter setValue:image forKey:kCIInputImageKey];
        [scaleFilter setValue:@(0.0) forKey:kCIInputSaturationKey];
    
        // 获取输出图像
        CIImage *outputImage = [scaleFilter outputImage];
       
        
        return outputImage;
    }
    
    
};

#endif /* GrayFilter_hpp */
