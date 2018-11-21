//
//  main.cpp
//  SDL_Audio
//
//  Created by birneysky on 2018/11/21.
//  Copyright © 2018 Grocery. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>

int main(int argc, const char * argv[]) {

    if(SDL_Init(SDL_INIT_AUDIO)) {
        std::cout << "Failed to initial!" << std::endl;
        return -1;
    }
    
    std::ifstream audioFile = std::ifstream("1.pcm");
    
    return 0;
}
