//
//  ViewController.m
//  ApiDemo
//
//  Created by birneysky on 2018/7/17.
//  Copyright © 2018年 Grocery. All rights reserved.
//

#import "ViewController.h"
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/pixdesc.h>

@interface ViewController ()

@end

static int interruptCallBack(void* arg) {
    return 0;
}

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
}



#pragma mark - Target Action

- (IBAction)openAction:(id)sender {
    /// 打开媒体文件，可能是本地磁盘的文件，也可能是网络媒体资源的一个连接
    /// 网络连接会涉及不同的协议，比如RTMP，HTTP等协议的视频源
    NSString* path = [[NSBundle mainBundle] pathForResource:@"test" ofType:@"MP4"];
    AVFormatContext* formatContext = avformat_alloc_context();
    AVIOInterruptCB interruptCb = {interruptCallBack,(__bridge void *)(self)};
    formatContext->interrupt_callback = interruptCb;
    avformat_open_input(&formatContext, path.UTF8String, NULL, NULL);
    avformat_find_stream_info(formatContext, NULL);
    
    /// 寻找音视频流:
    for (int i = 0; i < formatContext->nb_streams; i++) {
        AVStream* stream = formatContext->streams[i];
        if (AVMEDIA_TYPE_VIDEO == stream->codecpar->codec_type) {
            NSLog(@"videostreamIndex %d",i);
            AVCodecParameters* videoCodeParameter = stream->codecpar;
            AVCodec* codec = avcodec_find_decoder(videoCodeParameter->codec_id);
            if (!codec) {
                NSLog(@"not found video decoder");
            } else {
                AVCodecContext* codexCtx = avcodec_alloc_context3(codec);
                avcodec_parameters_to_context(codexCtx, videoCodeParameter);
                if (avcodec_open2(codexCtx, codec, NULL) < 0) {
                    NSLog(@"open video decoder failed");
                } else {
                    
                }
            }
        } else if(AVMEDIA_TYPE_AUDIO == stream->codecpar->codec_type) {
            NSLog(@"audiostreamIndex %d",i);
            ///打开音频流解码器
            AVCodecParameters* audioCodeParameter = stream->codecpar;
            AVCodec* codec = avcodec_find_decoder(audioCodeParameter->codec_id);
            if (!codec) {
                NSLog(@"not found audio decoder");
            } else {
                AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
                avcodec_parameters_to_context(codecCtx, audioCodeParameter);
                if(avcodec_open2(codecCtx, codec, NULL) < 0 ) {
                    NSLog(@"open audio decoder failed");
                } else {
                    
                }
            }
        }
    }
}



@end
