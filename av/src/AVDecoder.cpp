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
    codecCtx = nullptr;
    swsCtx = nullptr;
}

bool AVDecoder::open() {
    AVCodec* codec = avcodec_find_decoder(codecParam->codec_id);
    if (!codec) {
        av_log(nullptr, AV_LOG_ERROR,
               "codec_id %d was not found \n",
               codecParam->codec_id);
        return false;
    }
    codecCtx  = avcodec_alloc_context3(codec);
    if (!codecCtx) {
        av_log(nullptr, AV_LOG_ERROR,
               "codec context alloc failde \n");
        return false;
    }
    avcodec_parameters_to_context(codecCtx,codecParam);
    if(avcodec_open2(codecCtx, codec, nullptr) < 0 ) {
        av_log(nullptr, AV_LOG_ERROR,
               "open decoder failed,codec_id %d \n",
               codecParam->codec_id);
        return false;
    }
    return true;
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
    int code = avcodec_send_packet(codecCtx, packet);
    if(code != 0 ) {
        av_log(nullptr, AV_LOG_ERROR, "avcode_send_packet error %s \n",av_err2str(code));
        return nullptr;
    }
    AVFrame* avFrame = av_frame_alloc();
    code = avcodec_receive_frame(codecCtx,avFrame);
    if(code != 0) {
        av_log(nullptr, AV_LOG_ERROR, "avcodec_receive_frame error %s \n",av_err2str(code));
        av_frame_free(&avFrame);
        return nullptr;
    }
    
    return avFrame;
}

enum AVPixelFormat AVDecoder::getPixelFormat() {
    return codecCtx->pix_fmt;
}
