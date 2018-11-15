//
//  AVDecoder.cpp
//  AV
//
//  Created by birneysky on 2018/11/14.
//  Copyright © 2018 Grocery. All rights reserved.
//

#include "AVDecoder.hpp"


AVDecoder::AVDecoder(AVCodecParameters* parameters) {
    codecParam = parameters;
}

bool AVDecoder::open() {
    AVCodec* codec = avcodec_find_decoder(codecParam->codec_id);
    if (!codec) {
        av_log(nullptr, AV_LOG_ERROR,
               "codec_id %d was not found",
               codecParam->codec_id);
        return false;
    }
    codecCtx  = avcodec_alloc_context3(codec);
    if (!codecCtx) {
        av_log(nullptr, AV_LOG_ERROR,
               "codec context alloc failde");
        return false;
        return false;
    }
    avcodec_parameters_to_context(codecCtx,codecParam);
    if(avcodec_open2(codecCtx, codec, NULL) < 0 ) {
        av_log(nullptr, AV_LOG_ERROR,
               "open decoder failed,codec_id %d",
               codecParam->codec_id);
    }
    return false;
}
bool AVDecoder::close() {
    if (codecCtx) {
        avcodec_close(codecCtx);
        av_free(codecCtx);
        codecCtx = nullptr;
    }
    
    return false;
}

AVDecoder::~AVDecoder() {
    if (codecCtx) {
        avcodec_close(codecCtx);
        av_free(codecCtx);
        codecCtx = nullptr;
    }
}

AVFrame* AVDecoder::decode(AVPacket* packet) {
    if(avcodec_send_packet(codecCtx, packet) != 0 ) {
        return nullptr;
    }
    AVFrame* avFrame = av_frame_alloc();
    if(avcodec_receive_frame(codecCtx,avFrame) != 0) {
        av_frame_free(&avFrame);
        return nullptr;
    }
    
    return avFrame;
}
