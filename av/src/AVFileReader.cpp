//
//  AVFileReader.cpp
//  AV
//
//  Created by birney on 2018/11/15.
//  Copyright © 2018年 Grocery. All rights reserved.
//

#include "AVFileReader.hpp"
#include <iostream>

static int avio_interruptCallBack(void* arg) {
    av_log(nullptr, AV_LOG_INFO, "avio_interruptCallBack arg %p",arg);
    return 0;
}

AVFileReader::AVFileReader(const std::string& path) {
    localPath = path;
    formatCtx = avformat_alloc_context();
    AVIOInterruptCB interruptCb = {avio_interruptCallBack,this};
    formatCtx->interrupt_callback = interruptCb;
    
}

AVFileReader::~AVFileReader() {
    avformat_free_context(formatCtx);
    std::cout << "~AVFileReader" << std::endl;
}


bool AVFileReader::startReading() {
    int code = avformat_open_input(&formatCtx, localPath.c_str(), NULL, NULL);
    if (code != 0) {
        av_log(nullptr,AV_LOG_ERROR,
               "avfomart open input failed path:%s,code:%d,errormsg:%s",
               localPath.c_str(),code,av_err2str(code));
        return false;
    }
    code = avformat_find_stream_info(formatCtx, NULL);
    if (code < 0) {
        av_log(nullptr,AV_LOG_ERROR,
               "avfomart find stream info failed path:%s,code:%d,errormsg:%s",
               localPath.c_str(),code,av_err2str(code));
        return false;
    }
    return true;
}

bool AVFileReader::stopReading() {
    avformat_close_input(&formatCtx);
    return true;
}

AVPacket* AVFileReader::readNextFrame() {
    AVPacket* packet = av_packet_alloc();
    av_init_packet(packet);
    int code = av_read_frame(formatCtx, packet);
    if (code < 0) {
        av_log(nullptr, AV_LOG_ERROR, "av read frame failed code:%d errorMsg:%s",code,av_err2str(code));
        return nullptr;
    }
    return packet;
}
