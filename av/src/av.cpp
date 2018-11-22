#include <iostream>
#include "av_file.h"
#include "AVFileReader.hpp"
#include "AVDecoder.hpp"

extern "C"{
    #include <libavutil/log.h>
    #include <SDL2/SDL.h>
    #include <libavformat/avformat.h>
}
//using namespace std;

int main( int argc, char *argv[] ) {
    av_log_set_level(AV_LOG_INFO);
    av_log(NULL, AV_LOG_INFO, "%s\n", "Hello AV World");
    if (argc < 2 ){
        av_log(nullptr,AV_LOG_ERROR, "argc less than 2 \n");
        return -1;
    }
    AVFileReader reader = AVFileReader(argv[1]);
    if(!reader.startReading()) {
        av_log(nullptr, AV_LOG_ERROR, "open file error");
        return -1;
    }
    AVDecoder* videoDecoder = nullptr;
    AVDecoder* audioDecoder = nullptr;
    int videoStreamIndex = -1;
    int audioStreamIndex = -1;
    int videoWidth = 0;
    int videoHeight = 0;
    unsigned int streamCount = reader.getStreamCount();
    for (int i = 0; i < streamCount; i++) {
        AVStream* stream =  reader.getStream(i);
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoDecoder = new AVDecoder(stream->codecpar);
            videoStreamIndex = i;
            videoWidth = stream->codecpar->width;
            videoHeight = stream->codecpar->height;
        } else if(stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO ) {
            audioDecoder = new AVDecoder(stream->codecpar);
            audioStreamIndex = i;
        }
    }
    
    videoDecoder->open();
    audioDecoder->open();
    
    /// 初始化SDL
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* window = SDL_CreateWindow("SDL2 Window",
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            videoWidth,
                                            videoHeight,
                                            SDL_WINDOW_SHOWN );
    
    if (!window) {
        av_log(nullptr, AV_LOG_ERROR, "Faliled to create windows !");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    /// 创建渲染器 CreateRender
    SDL_Renderer* render =  SDL_CreateRenderer(window, -1, 0);
    if (!render) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    
    bool quit = true;
    SDL_Texture* texture = SDL_CreateTexture(render,
                                             SDL_PIXELFORMAT_IYUV,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             videoWidth,
                                             videoHeight);
    SDL_Rect rect;
    rect.w = videoWidth;
    rect.h = videoHeight;
    if (!texture) {
        av_log(nullptr, AV_LOG_ERROR, "create texture failed \n");
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(render);
        SDL_Quit();
        return -1;
    }
    while (quit) {

        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                quit = false;
                break;
            default:
                av_log(nullptr, AV_LOG_INFO, "event type is %d \n", event.type);
        }
        
        SDL_SetRenderTarget(render, texture);
        AVPacket* packet =  reader.readNextFrame();
        if (packet != nullptr) {
            if (packet->stream_index == videoStreamIndex) {
                av_log(nullptr, AV_LOG_INFO, "视频包 \n");
                AVFrame* videoFrame = videoDecoder->decode(packet);
                if (videoFrame) {
                    SDL_UpdateYUVTexture(texture,nullptr,
                                         videoFrame->data[0],
                                         videoFrame->linesize[0],
                                         videoFrame->data[1],
                                         videoFrame->linesize[1],
                                         videoFrame->data[2],
                                         videoFrame->linesize[2]);
                } else {
                    SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0x00);
                    SDL_RenderClear(render);
                    SDL_RenderDrawRect(render,&rect);
                    SDL_SetRenderDrawColor(render, 0xFF, 0x00, 0x00, 0x00);
                    av_log(nullptr, AV_LOG_ERROR, "Video decode error \n");
                }
                av_frame_free(&videoFrame);
            } else if(packet->stream_index == audioStreamIndex ){
                av_log(nullptr, AV_LOG_INFO, "音频包 \n");
            } else {
                av_log(nullptr, AV_LOG_INFO, "其他包 \n");
            }
        } else {
            av_log(nullptr, AV_LOG_INFO, "读取结束");
        }
        av_packet_free(&packet);

        SDL_RenderClear(render);
        SDL_RenderCopy(render, texture, nullptr, &rect) ;
        SDL_RenderPresent(render);
    }
    

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
