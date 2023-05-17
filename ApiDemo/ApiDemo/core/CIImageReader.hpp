//
//  CIImageReader.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/11.
//

#ifndef CIImageReader_hpp
#define CIImageReader_hpp

#include "NV12Reader.hpp"

#include <CoreVideo/CoreVideo.h>
#include <CoreImage/CoreImage.h>
//#include <AVFoundation/AVFoundation.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
}

class CIImageReader: public Reader<CIImage> {
private:
    NV12Reader* reader = nullptr;
public:
    
    CIImageReader(NV12Reader* reader): reader(reader) {
        
    }
    
    CIImage* read() override {
        AVFrame* frame = reader->read();
        if (!frame) {
            return nullptr;
        }
        NSDictionary *pixelAttributes = @{(id)kCVPixelBufferIOSurfacePropertiesKey : @{},
            (id)kCVPixelBufferBytesPerRowAlignmentKey:@(16),
            (id)kCVPixelBufferWidthKey:@(reader->getWidth()),
            (id)kCVPixelBufferHeightKey:@(reader->getHeight())
        };
        
        CVPixelBufferRef pixelBuffer = NULL;
        
        CVReturn result = CVPixelBufferCreate(kCFAllocatorDefault,
                                              reader->getWidth(),
                                              reader->getHeight(),
                                              kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange,
                                              (__bridge CFDictionaryRef)(pixelAttributes),
                                              &pixelBuffer);
        
    //    const int ySize = getWidth() * getHeight();
    //    const int uvSize = ySize / 2;
        CVPixelBufferLockBaseAddress(pixelBuffer,0);
        size_t bytesPerRowY = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer,0);
        size_t bytesPerRowUV = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer,1);
        uint8_t* yData = frame->data[0];
        uint8_t* uvData = frame->data[1];
        uint8_t *yDestPlane = (uint8_t*)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
        uint8_t *uvDestPlane = (uint8_t*)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
        for (int i = 0; i < reader->getHeight(); i++) {
            memcpy(yDestPlane, yData, frame->linesize[0]);
            yDestPlane +=  bytesPerRowY;
            yData += frame->linesize[0];
        }
        
        for (int i = 0; i < reader->getHeight() / 2; i++) {
            memcpy(uvDestPlane, uvData, frame->linesize[1]);
            uvDestPlane += bytesPerRowUV;
            uvData += frame->linesize[1];
        }
        
        CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
        
        if (result != kCVReturnSuccess) {
            NSLog(@"Unable to create cvpixelbuffer %d", result);
        }
        
        // CIImage Conversion
        
    //    NSDictionary *options = @{(id)kCIImageColorSpace: (__bridge id)CGColorSpaceCreateDeviceRGB()};
    //    CIImage *coreImage = [CIImage imageWithCVPixelBuffer:pixelBuffer options:options];
        CIImage *coreImage = [CIImage imageWithCVPixelBuffer:pixelBuffer];
        CVPixelBufferRelease(pixelBuffer);

        return coreImage;
    }
};

#endif /* CIImageReader_hpp */
