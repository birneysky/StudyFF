//
//  GLTextureReader.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/15.
//

#ifndef GLTextureReader_hpp
#define GLTextureReader_hpp

#include "Reader.hpp"
#include "GLTextureFrame.hpp"
#include "NV12Reader.hpp"
#include "GLTextureFrame.hpp"
#include "TextureRender.hpp"

class GLTextureReader: public Reader<GLTextureFrame> {
private:
    I420Reader* reader = nullptr;
    EAGLContext* _context =  nullptr;
    GLTextureFrame* _textureFrame;
    TextureRender* render = nullptr;
    EAGLContext* _glContext;
    CVPixelBufferRef pixelBuffer = NULL;
public:
    GLTextureReader(I420Reader* reader, EAGLContext* glContext): reader(reader), _glContext(glContext){

    }
    
    GLTextureFrame* read() override {
        AVFrame* frame = reader->read();
        if (!frame) {
            return nullptr;
        }
        
//        NSDictionary *pixelAttributes = @{(id)kCVPixelBufferIOSurfacePropertiesKey : @{},
//            (id)kCVPixelBufferBytesPerRowAlignmentKey:@(16),
//            (id)kCVPixelBufferWidthKey:@(reader->getWidth()),
//            (id)kCVPixelBufferHeightKey:@(reader->getHeight())
//        };
//
//
//
//        CVReturn result = CVPixelBufferCreate(kCFAllocatorDefault,
//                                              reader->getWidth(),
//                                              reader->getHeight(),
//                                              kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange,
//                                              (__bridge CFDictionaryRef)(pixelAttributes),
//                                              &pixelBuffer);
//
//        CVPixelBufferLockBaseAddress(pixelBuffer,0);
//        size_t bytesPerRowY = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer,0);
//        size_t bytesPerRowUV = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer,1);
//        uint8_t* yData = frame->data[0];
//        uint8_t* uvData = frame->data[1];
//        uint8_t *yDestPlane = (uint8_t*)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
//        uint8_t *uvDestPlane = (uint8_t*)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
//        for (int i = 0; i < reader->getHeight(); i++) {
//            memcpy(yDestPlane, yData, frame->linesize[0]);
//            yDestPlane +=  bytesPerRowY;
//            yData += frame->linesize[0];
//        }
//
//        for (int i = 0; i < reader->getHeight() / 2; i++) {
//            memcpy(uvDestPlane, uvData, frame->linesize[1]);
//            uvDestPlane += bytesPerRowUV;
//            uvData += frame->linesize[1];
//        }
//
//        CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
//
//        if (result != kCVReturnSuccess) {
//            NSLog(@"Unable to create cvpixelbuffer %d", result);
//        }

        if (!render) {
            render = new TextureRender(_glContext);
        }
        
        if (render) {
            //render->render(pixelBuffer);
            render->render(frame);
        }

        return render->outputTexture();
    }

};
#endif /* GLTextureReader_hpp */
