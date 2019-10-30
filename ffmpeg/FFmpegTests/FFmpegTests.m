//
//  FFmpegTests.m
//  FFmpegTests
//
//  Created by birneysky on 2019/10/30.
//  Copyright © 2019 Grocery. All rights reserved.
//

#import <XCTest/XCTest.h>
#import <Foundation/Foundation.h>
#include <libavutil/log.h>
#include <libavformat/avformat.h>

@interface FFmpegTests : XCTestCase

@end


@implementation FFmpegTests

+ (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
    //av_register_all();
    av_log_set_level(AV_LOG_INFO);
}

+ (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testDumpFileInfo {
    NSBundle* currentBundle = [NSBundle bundleForClass:self.class];
    NSString* path = [currentBundle  pathForResource:@"test" ofType:@"MP4"];
    AVFormatContext* fmt_ctx = NULL;
    int ret = avformat_open_input(&fmt_ctx, path.UTF8String, NULL, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't open file: %s", av_err2str(ret));
        return;
    }
    av_dump_format(fmt_ctx, 0, path.UTF8String, 0);
    avformat_close_input(&fmt_ctx);
    
}


const uint8_t ADTS_HEADER_LEN = 7;

void adts_header(char *szAdtsHeader, int dataLen){

    int audio_object_type = 2;
    int sampling_frequency_index = 7;
    int channel_config = 2;

    int adtsLen = dataLen + 7;

    szAdtsHeader[0] = 0xff;         //syncword:0xfff                          高8bits
    szAdtsHeader[1] = 0xf0;         //syncword:0xfff                          低4bits
    szAdtsHeader[1] |= (0 << 3);    //MPEG Version:0 for MPEG-4,1 for MPEG-2  1bit
    szAdtsHeader[1] |= (0 << 1);    //Layer:0                                 2bits
    szAdtsHeader[1] |= 1;           //protection absent:1                     1bit

    szAdtsHeader[2] = (audio_object_type - 1)<<6;            //profile:audio_object_type - 1                      2bits
    szAdtsHeader[2] |= (sampling_frequency_index & 0x0f)<<2; //sampling frequency index:sampling_frequency_index  4bits
    szAdtsHeader[2] |= (0 << 1);                             //private bit:0                                      1bit
    szAdtsHeader[2] |= (channel_config & 0x04)>>2;           //channel configuration:channel_config               高1bit

    szAdtsHeader[3] = (channel_config & 0x03)<<6;     //channel configuration:channel_config      低2bits
    szAdtsHeader[3] |= (0 << 5);                      //original：0                               1bit
    szAdtsHeader[3] |= (0 << 4);                      //home：0                                   1bit
    szAdtsHeader[3] |= (0 << 3);                      //copyright id bit：0                       1bit
    szAdtsHeader[3] |= (0 << 2);                      //copyright id start：0                     1bit
    szAdtsHeader[3] |= ((adtsLen & 0x1800) >> 11);           //frame length：value   高2bits

    szAdtsHeader[4] = (uint8_t)((adtsLen & 0x7f8) >> 3);     //frame length:value    中间8bits
    szAdtsHeader[5] = (uint8_t)((adtsLen & 0x7) << 5);       //frame length:value    低3bits
    szAdtsHeader[5] |= 0x1f;                                 //buffer fullness:0x7ff 高5bits
    szAdtsHeader[6] = 0xfc;
}

- (void)testExtractAudioData {
    ///API  av_init_packet   av_find_best_stream av_read_frame  av_packet_unref
    
    NSBundle* currentBundle = [NSBundle bundleForClass:self.class];
    NSString* srcPath = [currentBundle  pathForResource:@"test" ofType:@"MP4"];
    AVFormatContext* fmt_ctx = NULL;
    int ret = avformat_open_input(&fmt_ctx, srcPath.UTF8String, NULL, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't open file: %s", av_err2str(ret));
        return;
    }
    
    int audio_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    XCTAssert(audio_index >= 0, "find best stream failure: %s",av_err2str(ret));
    AVStream* audio_stream =  fmt_ctx->streams[audio_index];
    const AVCodecDescriptor* desc = avcodec_descriptor_get(audio_stream->codecpar->codec_id);
    NSLog(@"audio codec name: %s",desc->name);
    
    long long millSeconds =  [[NSDate date] timeIntervalSince1970] * 1000;
    NSString* destPath = [NSTemporaryDirectory() stringByAppendingFormat:@"%lld.%s",millSeconds,desc->name];
    NSLog(@"destPath = %@",destPath);
    
    AVPacket pkt;
    av_init_packet(&pkt);
    
    NSURL* destURL = [NSURL fileURLWithPath:destPath];
    NSOutputStream* audio_output = [[NSOutputStream alloc] initWithURL:destURL append:YES];
    [audio_output open];
    while (av_read_frame(fmt_ctx, &pkt) >= 0) {
        if (pkt.stream_index == audio_index) {
            char adts_header_buf[ADTS_HEADER_LEN] = {};
            adts_header(adts_header_buf, pkt.size);
            [audio_output write:(const uint8_t*)adts_header_buf maxLength:ADTS_HEADER_LEN];
            
            NSInteger len = [audio_output write:pkt.data maxLength:pkt.size];
            if (len != pkt.size) {
                XCTAssert(false);
                NSLog(@"write failed");
            }
        }
        av_packet_unref(&pkt);
    }
    [audio_output close];
    avformat_close_input(&fmt_ctx);
    
}



@end
