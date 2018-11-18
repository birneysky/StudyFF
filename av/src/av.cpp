#include <iostream>
#include "av_file.h"
#include "test/test.h"
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
    ////  p1->testFileStream();
    AVFileReader reader = AVFileReader(argv[1]);
    if(!reader.startReading()) {
        av_log(nullptr, AV_LOG_ERROR, "open file error");
    }
    
    /// 初始化SDL
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* window = SDL_CreateWindow("SDL2 Window",
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            640,
                                            480,
                                            SDL_WINDOW_SHOWN );
    
    if (!window) {
        av_log(nullptr, AV_LOG_ERROR, "Faliled to create windows !");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    /// 创建渲染器 CreateRender
    SDL_Renderer* render =  SDL_CreateRenderer(window, -1, 0);

    /// 销毁渲染器
    if (!render) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    /// 清空渲染器

    /// sdl 所有的事件都存放在一个队列中
    /// 所有对事件的操作，其实就是对队列的操作
    ///  事件类型
        /// SDL_WindowEvent :窗口事件
        /// SDL_KeyboardEvent: 键盘事件
        /// SDL_MouseMotionEvent: 鼠标事件
    /// 事件的两种方式 SDL_PoolEvent 轮询  SDL_WaitEvent 等待触发，
    ///SDL_Delay(30000);
    bool quit = true;
   SDL_Texture* texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 640, 480);
    SDL_Rect rect;
    rect.w = 50;
    rect.h = 50;
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
        
        AVPacket* packet =  reader.readNextFrame();
        AVCodecParameters* par = reader.getCodecParameters(packet->stream_index);
        rect.x = rand() % 600;
        rect.y = rand() % 400;
        av_log(nullptr, AV_LOG_INFO, "x %d, y %d \n",rect.x,rect.y);
        SDL_SetRenderTarget(render, texture);
        SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
        SDL_RenderClear(render);

        SDL_RenderDrawRect(render, &rect);
        SDL_SetRenderDrawColor(render, 255, 0, 0, 0);
        SDL_RenderFillRect(render, &rect);
        SDL_SetRenderTarget(render, nullptr);
        SDL_RenderCopy(render, texture, nullptr, nullptr) ;
        SDL_RenderPresent(render);
    }
    

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
