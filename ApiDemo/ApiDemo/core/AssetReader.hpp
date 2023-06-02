//
//  AssetReadeer.hpp
//  ApiDemo
//
//  Created by Bruce on 2023/5/18.
//  Copyright © 2023 Grocery. All rights reserved.
//

#ifndef AssetReadeer_hpp
#define AssetReadeer_hpp
#include <string>
#include <iostream>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include "GLTextureFrame.hpp"
#include "TextureRender.hpp"

class AssetReader {
public:
    enum class Status {unknown, reading, completed, failed, cancelled};
private:
    AVFormatContext *ifmt_ctx = nullptr;
    AVCodec *video_codec = nullptr;
    AVCodecContext *codec_ctx= nullptr;
    int video_stream_index = -1;
    std::string path;
    Status _status;
    TextureRender* render = nullptr;
    EAGLContext* _glContext = nullptr;
    
public:
    enum class MediaType {Video, Audio};
//    friend class class AssetReaderOutput<T>;
    AssetReader(const std::string& filePath, EAGLContext* glContext) {
        ifmt_ctx = avformat_alloc_context();
        path = filePath;
        _status = Status::unknown;
        _glContext = glContext;

    }
    
    ~AssetReader() {
        std::cout << "~AssetReader() " << this << std::endl;
        if (codec_ctx) {
            avcodec_close(codec_ctx);
            codec_ctx = nullptr;
        }
        
        if (ifmt_ctx) {
            avformat_close_input(&ifmt_ctx);
            ifmt_ctx = nullptr;
        }
        
        if (render) {
            delete render;
            render = nullptr;
        }
    }
    
    Status status()  const {
        return _status;
    }
    
    GLTextureFrame* getNextFrame(MediaType type) {
        BOOL exit = NO;
        while (!exit) {
            std::shared_ptr<AVPacket> packet(av_packet_alloc(), [](AVPacket* pkt) {
                av_packet_free(&pkt);
            });
            packet->data = NULL;
            packet->size = 0;
            int ret = av_read_frame(ifmt_ctx, packet.get());
            if (ret < 0) {
                return nullptr;
            }
            
            if (packet->stream_index == video_stream_index) {
                ret = avcodec_send_packet(codec_ctx, packet.get());
                if (ret < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Error sending a packet to the decoder\n");
                    break;
                }

                while (ret >= 0) {
                    std::shared_ptr<AVFrame> frame(av_frame_alloc(), [](AVFrame * frame){
                          av_frame_free(&frame);
                      });
                    ret = avcodec_receive_frame(codec_ctx, frame.get());
                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                        break;
                    }
                    if (ret < 0) {
                        break;
                    }
                    if (!render) {
                        render = new TextureRender(_glContext);
                    }
                    
                    if (render) {
                        render->render(frame.get());
                    }
                    exit = YES;
                    break;
                }
            }
        }
        return render->outputTexture();
    }
    
    bool startReading() {
        if (codec_ctx) {
            return false;
        }
        int ret = avformat_open_input(&ifmt_ctx, path.c_str(), NULL, NULL);
        if (ret  < 0) {
            std::cout << "cannot open input file" << path <<std::endl;
            return false;
        }
        
        ret = avformat_find_stream_info(ifmt_ctx, NULL);
        if (ret < 0) {
            std::cout << "cannot find input file stream information. file location:" << path << std::endl;
            return false;
        }
        
        video_stream_index = av_find_best_stream(ifmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
        if (video_stream_index < 0) {
            printf("Cannot find video stream in input file1\n");
            return false;
        }
        
        AVStream* video_stream = ifmt_ctx->streams[video_stream_index];
        // 创建解码器
        const AVCodec *codec = avcodec_find_decoder(video_stream->codecpar->codec_id);
        codec_ctx = avcodec_alloc_context3(codec);
        if (!codec_ctx) {
            av_log(NULL, AV_LOG_ERROR, "Failed to allocate the codec context\n");
            return false;
        }
        ret = avcodec_parameters_to_context(codec_ctx, video_stream->codecpar);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Failed to copy codec parameters to decoder context\n");
            return false;
        }
        ret = avcodec_open2(codec_ctx, codec, NULL);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Failed to open codec\n");
            return false;
        }
        
        _status = Status::reading;
        return true;
    }
    
    void cancelReading() {
        
    }
    
};

#endif /* AssetReadeer_hpp */
