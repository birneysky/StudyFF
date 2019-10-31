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
    AVStream* in_audio_stream =  fmt_ctx->streams[audio_index];
    const AVCodecDescriptor* desc = avcodec_descriptor_get(in_audio_stream->codecpar->codec_id);
    NSLog(@"audio codec name: %s",desc->name);
    
    long long millSeconds =  [[NSDate date] timeIntervalSince1970] * 1000;
    NSString* destPath = [NSTemporaryDirectory() stringByAppendingFormat:@"%lld.%s",millSeconds,desc->name];
    NSLog(@"destPath = %@",destPath);
    
    AVFormatContext* out_fmt_ctx = avformat_alloc_context();
    AVOutputFormat* out_fmt = av_guess_format(NULL, destPath.UTF8String, NULL);
    XCTAssert(out_fmt);
    out_fmt_ctx->oformat = out_fmt;
    AVStream* out_stream = avformat_new_stream(out_fmt_ctx, NULL);
    XCTAssert(out_stream);

    int error_code = avcodec_parameters_copy(out_stream->codecpar, in_audio_stream->codecpar);
    XCTAssert(error_code >= 0,
              @"Failed to copy codec parameter, %d(%s)\n",error_code,av_err2str(error_code));
    out_stream->codecpar->codec_tag = 0;
    
    
    
    error_code = avio_open(&out_fmt_ctx->pb, destPath.UTF8String, AVIO_FLAG_WRITE);
    XCTAssert(error_code >= 0, @"Can't open file %d(%s)",error_code,av_err2str(error_code));
    
    av_dump_format(out_fmt_ctx, 0, destPath.UTF8String, 1);
    
    error_code = avformat_write_header(out_fmt_ctx, NULL);
    XCTAssert(error_code >= 0,@"occured error when writing header %s",av_err2str(error_code));
    
    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;
    
    while (av_read_frame(fmt_ctx, &pkt) >= 0) {
        if (pkt.stream_index == audio_index) {
            pkt.pts = av_rescale_q_rnd(pkt.pts,
                                       in_audio_stream->time_base,
                                       out_stream->time_base,
                                       (AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
            pkt.dts = pkt.pts;
            pkt.duration = av_rescale_q(pkt.duration,
                                        in_audio_stream->time_base,
                                        out_stream->time_base);
            
            pkt.pos = -1;
            pkt.stream_index = 0;
            av_interleaved_write_frame(out_fmt_ctx, &pkt);
        }
        av_packet_unref(&pkt);
    }
    
    av_write_trailer(out_fmt_ctx);
    
    avformat_close_input(&fmt_ctx);
    avformat_free_context(out_fmt_ctx);
    
    avio_close(out_fmt_ctx->pb);

}



@end
