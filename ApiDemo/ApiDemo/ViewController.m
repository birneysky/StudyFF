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
#include <libavutil/imgutils.h>

@interface ViewController ()

@property(nonatomic,strong) NSOutputStream* output;

@end

static int interruptCallBack(void* arg) {
    return 0;
}

@implementation ViewController

#pragma mark - properties
- (NSOutputStream*)output {
    if (!_output) {
        long long millSeconds =  [[NSDate date] timeIntervalSince1970] * 1000;
        NSString* path = [NSTemporaryDirectory() stringByAppendingFormat:@"%lld.yuv",millSeconds];
        _output = [[NSOutputStream alloc] initWithURL:[NSURL fileURLWithPath:path] append:YES];
    }
    return _output;
}

#pragma mark - Life Cycle
- (void)viewDidLoad {
    [super viewDidLoad];
    [self.output open];
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
    
    int videoStreamIndex = 0;
    int audioStreamIndex = 0;
    AVCodecContext* audioCodecCtx = NULL;
    AVCodecContext* videoCodeCtx = NULL;
    /// 寻找音视频流:
    for (int i = 0; i < formatContext->nb_streams; i++) {
        AVStream* stream = formatContext->streams[i];
        if (AVMEDIA_TYPE_VIDEO == stream->codecpar->codec_type) {
            NSLog(@"videostreamIndex %d",i);
            videoStreamIndex = i;
            AVCodecParameters* videoCodeParameter = stream->codecpar;
            AVCodec* codec = avcodec_find_decoder(videoCodeParameter->codec_id);
            if (!codec) {
                NSLog(@"not found video decoder");
            } else {
                AVCodecContext* codexCtx = avcodec_alloc_context3(codec);
                avcodec_parameters_to_context(codexCtx, videoCodeParameter);
                videoCodeCtx = codexCtx;
                if (avcodec_open2(codexCtx, codec, NULL) < 0) {
                    NSLog(@"open video decoder failed");
                }
            }
        } else if(AVMEDIA_TYPE_AUDIO == stream->codecpar->codec_type) {
            NSLog(@"audiostreamIndex %d",i);
            audioStreamIndex = i;
            ///打开音频流解码器
            AVCodecParameters* audioCodeParameter = stream->codecpar;
            AVCodec* codec = avcodec_find_decoder(audioCodeParameter->codec_id);
            if (!codec) {
                NSLog(@"not found audio decoder");
            } else {
                AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
                avcodec_parameters_to_context(codecCtx, audioCodeParameter);
                audioCodecCtx = codecCtx;
                if(avcodec_open2(codecCtx, codec, NULL) < 0 ) {
                    NSLog(@"open audio decoder failed");
                }
            }
        }
    }
    /// 读取流内容，解码
    AVPacket packet;
    packet.data = NULL;
    packet.size = 0;
    av_init_packet(&packet);
    while (true) {
        if (av_read_frame(formatContext, &packet) < 0) {
            /// 小于0 出错或者读取完文件
            break;
        }
        int packetStreamIndex = packet.stream_index;
        if (packetStreamIndex == videoStreamIndex) {
            AVFrame* videoFrame = av_frame_alloc();
            
            //int gotFrame = 0;
            //avcodec_decode_video2(videoCodeCtx, videoFrame, &gotFrame, &packet);
            int result = avcodec_send_packet(videoCodeCtx, &packet);
            if (result != 0) {
                NSLog(@"video avcodec_send_packet");
            }
            AVFrame* yuvFrame = av_frame_alloc();
            int sizeYUV = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, videoCodeCtx->width, videoCodeCtx->height, 1);
            uint8_t *out_yuv_buffer = (uint8_t *)av_malloc(sizeYUV);
            av_image_fill_arrays(yuvFrame->data, yuvFrame->linesize, out_yuv_buffer, AV_PIX_FMT_YUV420P, videoCodeCtx->width, videoCodeCtx->height, 1);
            struct SwsContext *img_convert_ctx = sws_getContext(videoCodeCtx->width, videoCodeCtx->height, AV_PIX_FMT_YUV420P, videoCodeCtx->width, videoCodeCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
            if(avcodec_receive_frame(videoCodeCtx, videoFrame) == 0) {
                NSLog(@"video width %d, height %d",videoFrame->width,videoFrame->height);
                sws_scale(img_convert_ctx, videoFrame->data, videoFrame->linesize, 0, videoCodeCtx->height, yuvFrame->data, yuvFrame->linesize);
                if (videoCodeCtx->pix_fmt == AV_PIX_FMT_YUV420P || videoCodeCtx->pix_fmt == AV_PIX_FMT_YUVJ420P) {
                    
                    int videoSize = videoCodeCtx->width * videoCodeCtx->height;
                    uint8_t* yData = calloc(videoSize,1);
                    memcmp(yData, videoFrame->data[0], videoSize);
                    [self.output write:yData maxLength:videoSize];
                    uint8_t* uData = calloc(videoSize / 4,1);
                    memcmp(uData, videoFrame->data[1], videoSize / 4);
                    [self.output write:uData maxLength:videoSize/4];
                    uint8_t* vData = calloc(videoSize / 4,1);
                    memcmp(uData, videoFrame->data[2], videoSize / 4);
                    [self.output write:vData maxLength:videoSize / 4];
                    free(yData);
                    free(uData);
                    free(vData);
                    //videoFrame->data[0]
                } else {
                    
                }
            } else {
                NSLog(@"video decode failed");
            }
            av_frame_free(&videoFrame);
        } else if(packetStreamIndex == audioStreamIndex) {
            AVFrame* audioFrame = av_frame_alloc();
            //int gotFrame = 0;
            //int result = avcodec_decode_video2(audioCodecCtx, audioFrame, &gotFrame, &packet);
            int result = avcodec_send_packet(audioCodecCtx,&packet);
            if (result != 0) {
                NSLog(@"audio avcodec_send_packet failed");
            }
            if( avcodec_receive_frame(audioCodecCtx, audioFrame) == 0) {
                NSLog(@"audio width %d, height %d",audioFrame->width,audioFrame->height);
            } else {
                NSLog(@"audio decode failed %d",result);
            }
            av_frame_free(&audioFrame);
        }
    }
    
    avcodec_free_context(&videoCodeCtx);
    avcodec_free_context(&audioCodecCtx);
    avformat_free_context(formatContext);
    [self.output close];
}



@end
