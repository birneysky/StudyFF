//
//  AVDecoder.hpp
//  AV
//
//  Created by birneysky on 2018/11/14.
//  Copyright © 2018 Grocery. All rights reserved.
//

#ifndef AVDecoder_hpp
#define AVDecoder_hpp
#include <iostream>

extern "C"{
    #include <libavutil/log.h>
    #include <libavformat/avformat.h>
}

class AVDecoder {
private:
    AVCodecParameters* codecParam;
    AVCodecContext* codecCtx;
    struct SwsContext * swsCtx;
public:
    AVDecoder(AVCodecParameters* parameters);
    ~AVDecoder();
    bool open();
    bool close();
    enum AVPixelFormat getPixelFormat();
    AVFrame* decode(AVPacket* packet);
};


#endif /* AVDecoder_hpp */
