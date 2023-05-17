//
//  NV12Reader.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/11.
//

#ifndef NV12Reader_hpp
#define NV12Reader_hpp

#include "I420Reader.hpp"

class NV12Reader: public I420Reader {
public:
    NV12Reader(const std::string& path, const int& width, const int& height):I420Reader(path, width, height) {
        
    }
    
    virtual AVFrame* read() override {
        if (!file) {
            file = fopen(this->path.c_str(), "rb");
        }
        
        const AVPixelFormat pix_fmt = AV_PIX_FMT_NV12;
        
        if (!frame) {
            frame = av_frame_alloc();
        }
        frame->width = width;
        frame->height = height;
        frame->format = AV_PIX_FMT_NV12;
        
        const int ySize = width * height;
        const int uvSize = ySize / 2;
        
        int bufferSize = av_image_get_buffer_size(pix_fmt, width, height, 1);
        if (!buffer) {
            buffer = (uint8_t *)av_malloc(bufferSize);
            int ret = av_image_fill_arrays(frame->data, frame->linesize, buffer, pix_fmt, width, height, 1);
            //frame->linesize[1] = width / 2;
            if (ret < 0) {abort();}
        }
        
        AVRational frameRate = {25, 1};
        AVRational timeBase = av_inv_q(frameRate);
        //avpicture_fill((AVPicture*)frame, buffer, frame->format, frame->width, frame->height);
        if (fread(frame->data[0], 1, ySize, file) == ySize &&
            fread(frame->data[1], 1, uvSize, file) == uvSize) {
            frame->pts = av_rescale_q(frameIndex, timeBase, timeBase);
            frameIndex++;
            return frame;
        } else {
            return nullptr;
        }
    }
};

#endif /* NV12Reader_hpp */
