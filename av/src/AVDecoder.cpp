//
//  AVDecoder.cpp
//  AV
//
//  Created by birneysky on 2018/11/14.
//  Copyright © 2018 Grocery. All rights reserved.
//

#include "AVDecoder.hpp"


AVDecoder::AVDecoder(AVCodecParameters* parameters) {
    codecParam = new AVCodecParameters();
    memcpy(codecParam, parameters, (size_t)sizeof(parameters));
    AVCodec* codec = avcodec_find_decoder(codecParam->codec_id);
    codecCtx  = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecCtx,codecParam);
    if(avcodec_open2(codecCtx, codec, NULL) < 0 ) {
        av_log(nullptr, AV_LOG_ERROR, "open decoder failed,codec_id %d",codecParam->codec_id);
    }
    av_free(codec);
}

AVDecoder::~AVDecoder() {
    delete codecParam;
    avcodec_free_context(&codecCtx);
    
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
