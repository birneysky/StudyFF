//
//  I420Reader.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/9.
//

#ifndef I420Reader_hpp
#define I420Reader_hpp

#include <string>
#include <cstdio>
#include <cstdlib>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
}
#include "Reader.hpp"

class I420Reader: public Reader<AVFrame> {
protected:
    FILE *file = nullptr;
    int width = 0;
    int height = 0;
    std::string path = "";
    uint8_t *buffer = nullptr;
    AVFrame* frame = nullptr;
    int64_t frameIndex = 0;
public:
    I420Reader(const std::string& path, const int& width, const int& height) {
        this->path = path;
        this->width = width;
        this->height = height;
    }
    
    ~I420Reader() {
        if (frame) {
            av_frame_free(&frame);
            frame = nullptr;
        }
        
        if (buffer) {
            av_free(buffer);
            buffer = nullptr;
        }
    }
    
    
    int getWidth() {
        return this->width;
    }
    
    int getHeight() {
        return this->height;
    }
    
    virtual AVFrame* read() override {
        const AVPixelFormat pix_fmt = AV_PIX_FMT_YUV420P;
        
        if (!frame) {
            frame = av_frame_alloc();
        }
        frame->width = width;
        frame->height = height;
        frame->format = AV_PIX_FMT_YUV420P;
        
        const int ySize = width * height;
        const int uvSize = ySize / 4;
        
        int bufferSize = av_image_get_buffer_size(pix_fmt, width, height, 1);
        if (!buffer) {
            buffer = (uint8_t *)av_malloc(bufferSize);
            int ret = av_image_fill_arrays(frame->data, frame->linesize, buffer, pix_fmt, width, height, 1);
            if (ret < 0) {abort();}
        }
        
        AVRational frameRate = {25, 1};
        AVRational timeBase = av_inv_q(frameRate);
        //avpicture_fill((AVPicture*)frame, buffer, frame->format, frame->width, frame->height);
        if (fread(frame->data[0], 1, ySize, file) == ySize &&
            fread(frame->data[1], 1, uvSize, file) == uvSize &&
            fread(frame->data[2], 1, uvSize, file) == uvSize) {
            frame->pts = av_rescale_q(frameIndex, timeBase, timeBase);
            frameIndex++;
            return frame;
        } else {
            return nullptr;
        }
    }
};

#endif /* I420Reader_hpp */
