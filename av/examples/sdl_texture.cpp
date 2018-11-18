#include <SDL2/SDL.h>

int main (int argc, char *argv[] ) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;
    SDL_Rect r;

    /// 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    /// 创建窗口
    window = SDL_CreateWindow("SDL_CreateTexture",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    640, 480,
                    SDL_WINDOW_RESIZABLE);

    r.w = 50;
    r.h = 50;
    
    /// 创建渲染器 CreateRender
    renderer = SDL_CreateRenderer(window, -1, 0);

    /*  纹理渲染
            渲染器                 交换
            内存图像 -------->  纹理 -------- > 窗口展示
     
             纹理相关api
             SDL_CreateTexture()
             format:YUV,RGB
             access: Texture Target ,stream
             SDL_DestoryTexture()
             渲染相关的api
             SDL_SetRenderTarget() ///  设置渲染目标
             SDL_RenderClear() /// 清屏，
             SDL_RenderCopy ///  将纹理拷贝的显卡
             SDL_RenderPresent() /// 将图形渲染显示
     */
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);

    while (1) {
    /* sdl 所有的事件都存放在一个队列中 所有对事件的操作，其实就是对队列的操作
            事件类型
                SDL_WindowEvent :窗口事件
                SDL_KeyboardEvent: 键盘事件
                SDL_MouseMotionEvent: 鼠标事件
            事件的两种方式 SDL_PoolEvent 轮询  SDL_WaitEvent 等待触发，
            SDL_Delay(30000);
     */
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT)
                break;
        r.x=rand()%600;
        r.y=rand()%400;
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_RenderDrawRect(renderer,&r);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
        SDL_RenderFillRect(renderer, &r);
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
