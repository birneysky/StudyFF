//
//  ffmpeg.m
//  ffmpeg
//
//  Created by birneysky on 2018/7/12.
//  Copyright © 2018年 Grocery. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <libavformat/avformat.h>

void initFFmpeg() {
    avformat_network_init();
    av_register_all();
}
