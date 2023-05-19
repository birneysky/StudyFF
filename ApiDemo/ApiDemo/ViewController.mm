//
//  ViewController.m
//  ApiDemo
//
//  Created by birneysky on 2018/7/17.
//  Copyright © 2018年 Grocery. All rights reserved.
//

#import "ViewController.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/error.h>
#include <libavutil/mathematics.h>
#include <libavutil/pixdesc.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}


int pcmf32le_to_wave(const char *pcmpath, int channels, int sample_rate, const char *wavepath);

@interface ViewController ()
@property(nonatomic,strong) NSOutputStream* videoOutput;
@property(nonatomic,strong) NSOutputStream* audioOutput;
@property(nonatomic,copy) NSString* audioPath;
@end

static int interruptCallBack(void* arg) {
    return 0;
}

@implementation ViewController

#pragma mark - properties
- (NSOutputStream*)videoOutput {
    if (!_videoOutput) {
        long long millSeconds =  [[NSDate date] timeIntervalSince1970] * 1000;
        NSString* path = [NSTemporaryDirectory() stringByAppendingFormat:@"%lld.yuv",millSeconds];
        NSLog(@"path = %@",path);
        _videoOutput = [[NSOutputStream alloc] initWithURL:[NSURL fileURLWithPath:path] append:YES];
    }
    return _videoOutput;
}

- (NSOutputStream*)audioOutput {
    if (!_audioOutput) {
        _audioOutput = [[NSOutputStream alloc] initWithURL:[NSURL fileURLWithPath:self.audioPath] append:YES];
    }
    return _audioOutput;
}

- (NSString*)audioPath {
    if (!_audioPath) {
        long long millSeconds =  [[NSDate date] timeIntervalSince1970] * 1000;
        NSString* path = [NSTemporaryDirectory() stringByAppendingFormat:@"%lld.pcm",millSeconds];
        NSLog(@"path = %@",path);
        _audioPath = path;
    }
    return _audioPath;
}


#pragma mark - Life Cycle
- (void)viewDidLoad {
    [super viewDidLoad];
    [self.videoOutput open];
    [self.audioOutput open];
    [self testFilter];
    
    const AVCodec* codec  =  avcodec_find_encoder_by_name("h264_videotoolbox");
    const AVCodec* codec1 = avcodec_find_decoder_by_name("h264_videotoolbox");
    NSLog(@"codec %p", codec);
    
    /**
     
     dispatch_async(self.dispatch_queue, ^{
         
         EAGLContext *glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
         if (!glContext) {
           glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
         }
         
         self.glContext = glContext;
         
         [self.textRender setContext:self.glContext];
         [self.videoView setContext:self.glContext];

         NSString* yuvPath = [[NSBundle mainBundle] pathForResource:@"trailer_i420" ofType:@"yuv"];
         I420Reader i420_reader_test(yuvPath.UTF8String, 854, 480);
         GLTextureReader textReader(&i420_reader_test, glContext);
         GLFileInput glInput(textReader);
         GLScreen screen([self](GLTextureFrame* textFrame) {
                [self.textRender renderTexture:textFrame->getTexture() with:854 height:480];
         });
         glInput.connect(screen, 0);
         screen.start();
     });
     */
    
    
    //    dispatch_async(self.dispatch_queue, ^{
    //        NSString* yuvPath = [[NSBundle mainBundle] pathForResource:@"trailer_nv12" ofType:@"yuv"];
    //        NV12Reader nv12_reader0(yuvPath.UTF8String, 854, 480);
    //        NV12Reader nv12_reader1(yuvPath.UTF8String, 854, 480);
    //        CIImageReader reader0(&nv12_reader0);
    //        CIImageReader reader1(&nv12_reader1);
    //
    //        CIFileInput input0(reader0);
    //        CIFileInput input1(reader0);
    //        CIFilterScale scale;
    //        CIFilterOverlay overly;
    //        CIFilterGray gary;
    //
    //        CIScreen screen([self](CIImage* frame){
    //            [self.videoView renderFame:frame];
    //        });
    //
    //
    //        input0.connect(overly, 0);
    //        input1.connect(scale, 0);
    //        scale.connect(gary, 0);
    //        gary.connect(overly, 1);
    //        overly.connect(screen, 0);
    //        screen.start();
    //    });
    
}

void testfilters(void) {
    //av_register_all();
    AVFormatContext *ifmt_ctx1, *ifmt_ctx2;
    AVFormatContext *ofmt_ctx = NULL;
    AVStream *out_stream = NULL;
    AVCodecContext *ofmt_ctx_codec = NULL;
     AVCodec *video_codec = NULL;
    AVFilterContext *buffersink_ctx = NULL;
    AVFilterContext *buffersrc_ctx1 = NULL;
    AVFilterContext *buffersrc_ctx2 = NULL;
    AVFilterGraph *filter_graph = NULL;
    int video_stream_index1 = -1;
    int video_stream_index2 = -1;
    int ret;
    int frames = 0;
    
    
    // 打开输入文件1
    ifmt_ctx1 = avformat_alloc_context();
    NSString* path1 = [[NSBundle mainBundle] pathForResource:@"trailer" ofType:@"mp4"];
    if ((ret = avformat_open_input(&ifmt_ctx1, path1.UTF8String, NULL, NULL)) < 0) {
//        char buf[1024] = {};
//        char* error =av_strerror(ret, buf, sizeof(buf));
        printf("Cannot open input file1\n");
        return;
    }

    // 查找流信息
    if ((ret = avformat_find_stream_info(ifmt_ctx1, NULL)) < 0) {
        printf("Cannot find input file1 stream information\n");
        return;
    }

    // 打开输入文件2
    NSString* path2 = [[NSBundle mainBundle] pathForResource:@"trailer" ofType:@"mp4"];
    ifmt_ctx2 = avformat_alloc_context();
    if ((ret = avformat_open_input(&ifmt_ctx2, path2.UTF8String, NULL, NULL)) < 0) {
        printf("Cannot open input file2\n");
        return;
    }

    // 查找流信息
    if ((ret = avformat_find_stream_info(ifmt_ctx2, NULL)) < 0) {
        printf("Cannot find input file2 stream information\n");
        return;
    }

    // 打印输入文件1的流信息
    av_dump_format(ifmt_ctx1, 0, path1.UTF8String, 0);

    // 打印输入文件2的流信息
    av_dump_format(ifmt_ctx2, 0, path2.UTF8String, 0);

    // 创建输出文件
    NSString* path3 = [NSString stringWithFormat:@"%@%@",NSTemporaryDirectory(),@"124.mp4"];
    if ((ret = avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, path3.UTF8String)) < 0) {
        printf("Cannot allocate output context\n");
        return;
    }
    


    // 添加视频输出流
    video_stream_index1 = av_find_best_stream(ifmt_ctx1, AVMEDIA_TYPE_VIDEO, -1, -1, &video_codec, 0);
    if (video_stream_index1 < 0) {
        printf("Cannot find video stream in input file1\n");
        return;
    }
    
    AVStream* video_stream1 = ifmt_ctx1->streams[video_stream_index1];
    // 创建解码器
   const AVCodec *codec1 = avcodec_find_decoder(video_stream1->codecpar->codec_id);
   if (!codec1) {
       av_log(NULL, AV_LOG_ERROR, "Failed to find decoder for stream #%u\n",video_stream1);
       return;
   }
   AVCodecContext *codec_ctx1 = avcodec_alloc_context3(codec1);
   if (!codec_ctx1) {
       av_log(NULL, AV_LOG_ERROR, "Failed to allocate the codec context\n");
       return;
   }
   ret = avcodec_parameters_to_context(codec_ctx1, video_stream1->codecpar);
   if (ret < 0) {
       av_log(NULL, AV_LOG_ERROR, "Failed to copy codec parameters to decoder context\n");
       return;
   }
   ret = avcodec_open2(codec_ctx1, codec1, NULL);
   if (ret < 0) {
       av_log(NULL, AV_LOG_ERROR, "Failed to open codec\n");
       return;
   }
    
    // 添加视频输出流
    video_stream_index2 = av_find_best_stream(ifmt_ctx2, AVMEDIA_TYPE_VIDEO, -1, -1, &video_codec, 0);
    if (video_stream_index2 < 0) {
        printf("Cannot find video stream in input file1\n");
        return;
    }
    
    
    AVStream* video_stream2 = ifmt_ctx2->streams[video_stream_index2];
    // 创建解码器
   const AVCodec *codec2 = avcodec_find_decoder(video_stream2->codecpar->codec_id);
   if (!codec1) {
       av_log(NULL, AV_LOG_ERROR, "Failed to find decoder for stream #%u\n",video_stream1);
       return;
   }
    
   AVCodecContext *codec_ctx2 = avcodec_alloc_context3(codec2);
   if (!codec_ctx1) {
       av_log(NULL, AV_LOG_ERROR, "Failed to allocate the codec context\n");
       return;
   }
    
   ret = avcodec_parameters_to_context(codec_ctx2, video_stream1->codecpar);
   if (ret < 0) {
       av_log(NULL, AV_LOG_ERROR, "Failed to copy codec parameters to decoder context\n");
       return;
   }
    
   ret = avcodec_open2(codec_ctx2, codec1, NULL);
   if (ret < 0) {
       av_log(NULL, AV_LOG_ERROR, "Failed to open codec\n");
       return;
   }
    

    out_stream = avformat_new_stream(ofmt_ctx, NULL);
    out_stream->codecpar->width = ifmt_ctx1->streams[video_stream_index1]->codecpar->width;
    out_stream->codecpar->height = ifmt_ctx1->streams[video_stream_index1]->codecpar->height;
    out_stream->codecpar->codec_id = video_codec->id;
    out_stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    out_stream->codecpar->codec_id = AV_CODEC_ID_H264;
    out_stream->codecpar->format = AV_PIX_FMT_YUV420P;
    out_stream->codecpar->bit_rate = 400000;
    out_stream->time_base = (AVRational){1, 25};
    
    ofmt_ctx_codec = avcodec_alloc_context3(video_codec);
    avcodec_parameters_to_context(ofmt_ctx_codec, out_stream->codecpar);
    //out_stream->codec = ofmt_ctx_codec;
    
    // 初始化编码器
    const AVCodec *out_codec = avcodec_find_encoder(out_stream->codecpar->codec_id);
    if (!out_codec) {
        av_log(NULL, AV_LOG_ERROR, "Failed to find encoder for stream #%u\n", out_stream->index);
        return;
    }
    AVCodecContext *codec_ctx_out = avcodec_alloc_context3(out_codec);
    if (!codec_ctx_out) {
        av_log(NULL, AV_LOG_ERROR, "Failed to allocate the encoder context\n");
        return;
    }
    ret = avcodec_parameters_to_context(codec_ctx_out, out_stream->codecpar);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Failed to copy codec parameters to encoder context\n");
        return;
    }
    codec_ctx_out->time_base = out_stream->time_base;
    ret = avcodec_open2(codec_ctx_out, out_codec, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Failed to open encoder\n");
        return;
    }
    
    // 打开输出文件
       ret = avio_open(&ofmt_ctx->pb, path3.UTF8String, AVIO_FLAG_WRITE);
       if (ret < 0) {
           fprintf(stderr, "Error opening output file: %s\n", av_err2str(ret));
           return ;
       }

    
    // 写文件头
      ret = avformat_write_header(ofmt_ctx, NULL);
      if (ret < 0) {
          fprintf(stderr, "Error writing file header: %s\n", av_err2str(ret));
          return;
      }

    // 初始化滤镜图
       filter_graph = avfilter_graph_alloc();
       // 创建源滤镜
       const AVFilter *buffersrc = avfilter_get_by_name("buffer");
       char args[512];
       sprintf(args, "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
               ifmt_ctx1->streams[video_stream_index1]->codecpar->width,
               ifmt_ctx1->streams[video_stream_index1]->codecpar->height,
               ifmt_ctx1->streams[video_stream_index1]->codecpar->format,
               ifmt_ctx1->streams[video_stream_index1]->time_base.num, ifmt_ctx1->streams[video_stream_index1]->time_base.den,
               ifmt_ctx1->streams[video_stream_index1]->codecpar->sample_aspect_ratio.num,
               ifmt_ctx1->streams[video_stream_index1]->codecpar->sample_aspect_ratio.den);

       if ((ret = avfilter_graph_create_filter(&buffersrc_ctx1, buffersrc, "in1", args, NULL, filter_graph)) < 0) {
           printf("Cannot create buffer source filter for input file1\n");
           return;
       }

       sprintf(args, "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
               ifmt_ctx2->streams[video_stream_index2]->codecpar->width,
               ifmt_ctx2->streams[video_stream_index2]->codecpar->height,
               ifmt_ctx2->streams[video_stream_index2]->codecpar->format,
               ifmt_ctx2->streams[video_stream_index2]->time_base.num, ifmt_ctx2->streams[video_stream_index2]->time_base.den,
               ifmt_ctx2->streams[video_stream_index2]->codecpar->sample_aspect_ratio.num,
               ifmt_ctx2->streams[video_stream_index2]->codecpar->sample_aspect_ratio.den);

       if ((ret = avfilter_graph_create_filter(&buffersrc_ctx2, buffersrc, "in2", args, NULL, filter_graph)) < 0) {
           printf("Cannot create buffer source filter for input file2\n");
           return;
       }

       // 创建目标滤镜
        const AVFilter *buffersink = avfilter_get_by_name("buffersink");
        enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_YUV420P, AV_PIX_FMT_NONE };
        if ((ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out", NULL, NULL, filter_graph)) < 0) {
            printf("Cannot create buffer sink filter\n");
            return;
        }
    
    
    // 添加滤镜到滤镜图
    
//    AVFilterInOut *outputs2 = avfilter_inout_alloc();
//    outputs2->name = av_strdup("in2");
//    outputs2->filter_ctx = buffersrc_ctx2;
//    outputs2->pad_idx = 0;
//    outputs2->next = NULL;
//
//
//    AVFilterInOut *outputs = avfilter_inout_alloc();
//    outputs->name = av_strdup("in1");
//    outputs->filter_ctx = buffersrc_ctx1;
//    outputs->pad_idx = 0;
//    outputs->next = outputs2;
//
//
//
//    AVFilterInOut *inputs = avfilter_inout_alloc();
//    inputs->name = av_strdup("out");
//    inputs->filter_ctx = buffersink_ctx;
//    inputs->pad_idx = 0;
//    inputs->next = NULL;

//    char filter_descr[256];
//    snprintf(filter_descr, sizeof(filter_descr), "null");
    
    
    const AVFilter *scale = avfilter_get_by_name("scale");
    AVFilterContext *scale_ctx = nil;//avfilter_graph_alloc_filter(filter_graph, overlay, "overlay");
    
//    if ((ret = avfilter_graph_create_filter(&overlay_ctx, overlay, "overlay", NULL, NULL, filter_graph)) < 0) {
//        char errbuf[AV_ERROR_MAX_STRING_SIZE] = {};
//        av_strerror(ret, errbuf, AV_ERROR_MAX_STRING_SIZE);
//        printf("Cannot create filter\n");
//        return;
//    }
//    ret = avfilter_init_str(overlay_ctx, "overlay=x=(W-w)/2:y=(H-h)/2");
//    if (ret < 0) {
//        printf("Cannot create filter\n");
//        return;
//    }

    if ((ret = avfilter_graph_create_filter(&scale_ctx, scale, "scale", "176:h=144:force_original_aspect_ratio=1", NULL, filter_graph)) < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE] = {};
        av_strerror(ret, errbuf, AV_ERROR_MAX_STRING_SIZE);
        printf("Cannot create filter\n");
        return;
    }
    
    const AVFilter *overlay = avfilter_get_by_name("overlay");
    AVFilterContext *overlay_ctx = nil;//avfilter_graph_alloc_filter(filter_graph, overlay, "overlay");
    
//    if ((ret = avfilter_graph_create_filter(&overlay_ctx, overlay, "overlay", NULL, NULL, filter_graph)) < 0) {
//        char errbuf[AV_ERROR_MAX_STRING_SIZE] = {};
//        av_strerror(ret, errbuf, AV_ERROR_MAX_STRING_SIZE);
//        printf("Cannot create filter\n");
//        return;
//    }
//    ret = avfilter_init_str(overlay_ctx, "overlay=x=(W-w)/2:y=(H-h)/2");
//    if (ret < 0) {
//        printf("Cannot create filter\n");
//        return;
//    }

    if ((ret = avfilter_graph_create_filter(&overlay_ctx, overlay, "overlay", "x=10:y=60", NULL, filter_graph)) < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE] = {};
        av_strerror(ret, errbuf, AV_ERROR_MAX_STRING_SIZE);
        printf("Cannot create filter\n");
        return;
    }

//    if ((ret = avfilter_graph_parse_ptr(filter_graph, filter_descr, &inputs, &outputs, NULL)) < 0) {
//        printf("Cannot parse filter graph\n");
//        return;
//    }
    

    
    
    // 连接滤镜
      ret = avfilter_link(buffersrc_ctx1, 0, overlay_ctx, 0);
      if (ret < 0) {
          fprintf(stderr, "Cannot link buffer source1 to overlay\n");
          return;
      }
    
    
     ret = avfilter_link(buffersrc_ctx2, 0, scale_ctx, 0);
    if (ret < 0) {
        fprintf(stderr, "Cannot link buffer source2 to overlay\n");
        return;
    }
      ret = avfilter_link(scale_ctx, 0, overlay_ctx, 1);
      if (ret < 0) {
          fprintf(stderr, "Cannot link buffer source2 to overlay\n");
          return;
      }

      ret = avfilter_link(overlay_ctx, 0, buffersink_ctx, 0);
      if (ret < 0) {
          fprintf(stderr, "Cannot link overlay to buffer sink\n");
          return;
      }

      // 配置滤镜图
      ret = avfilter_graph_config(filter_graph, NULL);
      if (ret < 0) {
          fprintf(stderr, "Cannot configure filter graph\n");
          return;
      }
    
    
    // 读取帧和应用滤镜
    
    AVFrame *filterFrame = av_frame_alloc();
    AVFrame *filtered_frame = av_frame_alloc();
    
    AVPacket* packet1 = av_packet_alloc();
    packet1->data = NULL;
    packet1->size = 0;
    av_init_packet(packet1);
    
    
    AVPacket* packet2 = av_packet_alloc();
    packet2->data = NULL;
    packet2->size = 0;
    av_init_packet(packet2);

    
    while (true) {
        BOOL file1Done = NO;
        while (!file1Done) {
            ret = av_read_frame(ifmt_ctx1, packet1);
            if (ret < 0) {
                file1Done = YES;
                break;
            }
            
            if (packet1->stream_index == video_stream_index1) {
                ret = avcodec_send_packet(codec_ctx1, packet1);
                if (ret < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Error sending a packet to the decoder\n");
                    break;
                }

                BOOL exit1 = NO;
                while (ret >= 0) {
                    AVFrame *frame1 = av_frame_alloc();
                    ret = avcodec_receive_frame(codec_ctx1, frame1);
                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                        //av_frame_free(&frame1);
                        break;
                    }
                    if (ret < 0) {
                        //av_frame_free(&frame1);
                        return;
                    }

                    // 发送帧到输入滤镜1
                    if (av_buffersrc_add_frame_flags(buffersrc_ctx1, frame1, AV_BUFFERSRC_FLAG_PUSH) < 0) {
                        av_log(NULL, AV_LOG_ERROR, "Error while feeding the filtergraph\n");
                    } else {
                        av_frame_free(&frame1);
                        av_packet_unref(packet1);
                        exit1 = YES;
                    }
                    
                    break;
                }
                if(exit1) {
                    break;
                }
            }
        }
        
        BOOL file2Done = NO;
        while (!file2Done) {
            ret = av_read_frame(ifmt_ctx2, packet2);
            if (ret < 0) {
                file2Done = YES;
                break;
            }
            
            if (packet2->stream_index == video_stream_index2) {
                ret = avcodec_send_packet(codec_ctx2, packet2);
                if (ret < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Error sending a packet to the decoder\n");
                    break;
                }
                
                BOOL exit2 = NO;
                while (ret >= 0) {
                    AVFrame *frame2 = av_frame_alloc();
                    ret = avcodec_receive_frame(codec_ctx2, frame2);
                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                        //av_frame_free(&frame1);
                        break;
                    }
                    if (ret < 0) {
                        //av_frame_free(&frame1);
                        break;
                    }
                    
                    // 发送帧到输入滤镜1
                    if (av_buffersrc_add_frame_flags(buffersrc_ctx2, frame2, AV_BUFFERSRC_FLAG_PUSH) < 0) {
                        av_log(NULL, AV_LOG_ERROR, "Error while feeding the filtergraph\n");
                    } else {
                        av_frame_free(&frame2);
                        av_packet_unref(packet2);
                        exit2 = YES;
                    }
                    break;
                }
                if(exit2) {
                    break;
                }
            }
        }
        
        
        // 从滤镜图形中获取处理后的帧
        
        BOOL exit = NO;
    while (!exit) {
        
        if (file1Done && file2Done) {
            break;
        }
        
        ret = av_buffersink_get_frame_flags(buffersink_ctx, filterFrame, AV_BUFFERSRC_FLAG_KEEP_REF);//av_buffersink_get_frame(buffersink_ctx, filterFrame);
        if (ret == AVERROR_EOF) {
            break;;
        }
        
        if (ret == AVERROR(EAGAIN)) {
            break;
        }
        if (ret < 0) {
            break;
        }
        filterFrame->pts = av_rescale_q(filterFrame->pts, buffersink_ctx->inputs[0]->time_base, out_stream->time_base);
        filterFrame->pkt_dts = av_rescale_q(filterFrame->pkt_dts, buffersink_ctx->inputs[0]->time_base, out_stream->time_base);
        filterFrame->pkt_duration = av_rescale_q(filterFrame->pkt_duration, buffersink_ctx->inputs[0]->time_base, out_stream->time_base);
        
        ret = avcodec_send_frame(codec_ctx_out, filterFrame);
        if (ret < 0) {
            fprintf(stderr, "Error sending frame to codec: %s\n", av_err2str(ret));
        }
               while (ret >= 0) {
                   AVPacket* pkt = av_packet_alloc();
                    av_init_packet(pkt);
                    pkt->data = NULL;
                   pkt->size = 0;
                   ret = avcodec_receive_packet(codec_ctx_out, pkt);
                   if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                       break;
                   }
                   if (ret < 0) {
                       fprintf(stderr, "Error receiving packet from codec: %s\n", av_err2str(ret));
                       return;
                   }

                   // 写入帧
                   
//                   av_packet_rescale_ts(&pkt, codec_ctx_out->time_base, out_stream->time_base);
//                   pkt.stream_index = out_stream->index;
                   ret = av_interleaved_write_frame(ofmt_ctx, pkt);
                   if (ret < 0) {
                       fprintf(stderr, "Error writing frame to file: %s\n", av_err2str(ret));
                       return;
                   }
                   av_interleaved_write_frame(ofmt_ctx, NULL);
                   av_packet_free(&pkt);
               }

//        // 将帧写入输出文件

//        ret = av_interleaved_write_frame(ofmt_ctx, filterFrame);
//        if (ret < 0) {
//            return;
//        }
    }

        if (file1Done && file2Done) {
            av_write_trailer(ofmt_ctx);
            break;
        }
        
        

    }
    
       
    if (ifmt_ctx1) {
        avformat_close_input(&ifmt_ctx1);
    }

    if (ifmt_ctx2) {
        avformat_close_input(&ifmt_ctx2);
    }

    if (ofmt_ctx_codec) {
        avcodec_free_context(&ofmt_ctx_codec);
    }

    if (ofmt_ctx) {
        avformat_free_context(ofmt_ctx);
    }

    if (buffersrc_ctx1) {
        avfilter_free(buffersrc_ctx1);
    }
    
    if (buffersrc_ctx2) {
        avfilter_free(buffersrc_ctx2);
    }

    if (buffersink_ctx) {
        avfilter_free(buffersink_ctx);
    }
    
    if (scale_ctx) {
        avfilter_free(scale_ctx);
    }
    
    if (overlay_ctx) {
        avfilter_free(overlay_ctx);
    }

    if (filter_graph) {
        avfilter_graph_free(&filter_graph);
    }

}

- (void)testFilter {
    testfilters2();
}


void testfilters2(void) {
    AVFormatContext *ifmt_ctx1, *ifmt_ctx2;
    AVFormatContext *ofmt_ctx = NULL;
    AVStream *out_stream = NULL;
    AVCodecContext *ofmt_ctx_codec = NULL;
    AVCodec *video_codec = NULL;
    AVFilterContext *buffersink_ctx = NULL;
    AVFilterContext *buffersrc_ctx1 = NULL;
    AVFilterContext *buffersrc_ctx2 = NULL;
    AVFilterGraph *filter_graph = NULL;
    int video_stream_index1 = -1;
    int video_stream_index2 = -1;
    int ret;
    int frames = 0;
    
    
    // 打开输入文件1
    ifmt_ctx1 = avformat_alloc_context();
    NSString* path1 = [[NSBundle mainBundle] pathForResource:@"trailer" ofType:@"mp4"];
    if ((ret = avformat_open_input(&ifmt_ctx1, path1.UTF8String, NULL, NULL)) < 0) {
//        char buf[1024] = {};
//        char* error =av_strerror(ret, buf, sizeof(buf));
        printf("Cannot open input file1\n");
        return;
    }

    // 查找流信息
    if ((ret = avformat_find_stream_info(ifmt_ctx1, NULL)) < 0) {
        printf("Cannot find input file1 stream information\n");
        return;
    }

    // 打开输入文件2
    NSString* path2 = [[NSBundle mainBundle] pathForResource:@"trailer" ofType:@"mp4"];
    ifmt_ctx2 = avformat_alloc_context();
    if ((ret = avformat_open_input(&ifmt_ctx2, path2.UTF8String, NULL, NULL)) < 0) {
        printf("Cannot open input file2\n");
        return;
    }

    // 查找流信息
    if ((ret = avformat_find_stream_info(ifmt_ctx2, NULL)) < 0) {
        printf("Cannot find input file2 stream information\n");
        return;
    }

    // 打印输入文件1的流信息
    av_dump_format(ifmt_ctx1, 0, path1.UTF8String, 0);

    // 打印输入文件2的流信息
    av_dump_format(ifmt_ctx2, 0, path2.UTF8String, 0);

    // 创建输出文件
    NSString* path3 = [NSString stringWithFormat:@"%@%@",NSTemporaryDirectory(),@"124.mp4"];
    if ((ret = avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, path3.UTF8String)) < 0) {
        printf("Cannot allocate output context\n");
        return;
    }
    


    // 添加视频输出流
    
    video_stream_index1 = av_find_best_stream(ifmt_ctx1, AVMEDIA_TYPE_VIDEO, -1, -1, &video_codec, 0);
    if (video_stream_index1 < 0) {
        printf("Cannot find video stream in input file1\n");
        return;
    }
    
    AVStream* video_stream1 = ifmt_ctx1->streams[video_stream_index1];
    // 创建解码器
   const AVCodec *codec1 = avcodec_find_decoder(video_stream1->codecpar->codec_id);
   if (!codec1) {
       av_log(NULL, AV_LOG_ERROR, "Failed to find decoder for stream #%u\n",video_stream1);
       return;
   }
   AVCodecContext *codec_ctx1 = avcodec_alloc_context3(codec1);
   if (!codec_ctx1) {
       av_log(NULL, AV_LOG_ERROR, "Failed to allocate the codec context\n");
       return;
   }
   ret = avcodec_parameters_to_context(codec_ctx1, video_stream1->codecpar);
   if (ret < 0) {
       av_log(NULL, AV_LOG_ERROR, "Failed to copy codec parameters to decoder context\n");
       return;
   }
   ret = avcodec_open2(codec_ctx1, codec1, NULL);
   if (ret < 0) {
       av_log(NULL, AV_LOG_ERROR, "Failed to open codec\n");
       return;
   }
    
    // 添加视频输出流
    video_stream_index2 = av_find_best_stream(ifmt_ctx2, AVMEDIA_TYPE_VIDEO, -1, -1, &video_codec, 0);
    if (video_stream_index2 < 0) {
        printf("Cannot find video stream in input file1\n");
        return;
    }
    
    
    AVStream* video_stream2 = ifmt_ctx2->streams[video_stream_index2];
    // 创建解码器
   const AVCodec *codec2 = avcodec_find_decoder(video_stream2->codecpar->codec_id);
   if (!codec1) {
       av_log(NULL, AV_LOG_ERROR, "Failed to find decoder for stream #%u\n",video_stream1);
       return;
   }
    
   AVCodecContext *codec_ctx2 = avcodec_alloc_context3(codec2);
   if (!codec_ctx1) {
       av_log(NULL, AV_LOG_ERROR, "Failed to allocate the codec context\n");
       return;
   }
    
   ret = avcodec_parameters_to_context(codec_ctx2, video_stream1->codecpar);
   if (ret < 0) {
       av_log(NULL, AV_LOG_ERROR, "Failed to copy codec parameters to decoder context\n");
       return;
   }
    
   ret = avcodec_open2(codec_ctx2, codec1, NULL);
   if (ret < 0) {
       av_log(NULL, AV_LOG_ERROR, "Failed to open codec\n");
       return;
   }
    

    out_stream = avformat_new_stream(ofmt_ctx, NULL);
    out_stream->codecpar->width = ifmt_ctx1->streams[video_stream_index1]->codecpar->width;
    out_stream->codecpar->height = ifmt_ctx1->streams[video_stream_index1]->codecpar->height;
    out_stream->codecpar->codec_id = video_codec->id;
    out_stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    out_stream->codecpar->codec_id = AV_CODEC_ID_H264;
    out_stream->codecpar->format = AV_PIX_FMT_YUV420P;
    out_stream->codecpar->bit_rate = 400000;
    out_stream->time_base = (AVRational){1, 25};
    
    ofmt_ctx_codec = avcodec_alloc_context3(video_codec);
    avcodec_parameters_to_context(ofmt_ctx_codec, out_stream->codecpar);
    //out_stream->codec = ofmt_ctx_codec;
    
    

    
    // 初始化编码器
    const AVCodec *out_codec = avcodec_find_encoder(out_stream->codecpar->codec_id);
    if (!out_codec) {
        av_log(NULL, AV_LOG_ERROR, "Failed to find encoder for stream #%u\n", out_stream->index);
        return;
    }
    AVCodecContext *codec_ctx_out = avcodec_alloc_context3(out_codec);
    if (!codec_ctx_out) {
        av_log(NULL, AV_LOG_ERROR, "Failed to allocate the encoder context\n");
        return;
    }
    ret = avcodec_parameters_to_context(codec_ctx_out, out_stream->codecpar);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Failed to copy codec parameters to encoder context\n");
        return;
    }
    codec_ctx_out->time_base = out_stream->time_base;
    ret = avcodec_open2(codec_ctx_out, out_codec, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Failed to open encoder\n");
        return;
    }
    
    // 打开输出文件
       ret = avio_open(&ofmt_ctx->pb, path3.UTF8String, AVIO_FLAG_WRITE);
       if (ret < 0) {
           fprintf(stderr, "Error opening output file: %s\n", av_err2str(ret));
           return ;
       }

    
    // 写文件头
      ret = avformat_write_header(ofmt_ctx, NULL);
      if (ret < 0) {
          fprintf(stderr, "Error writing file header: %s\n", av_err2str(ret));
          return;
      }

    // 初始化滤镜图
       filter_graph = avfilter_graph_alloc();
       // 创建源滤镜
       const AVFilter *buffersrc = avfilter_get_by_name("buffer");
       char args[512];
       sprintf(args, "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
               ifmt_ctx1->streams[video_stream_index1]->codecpar->width,
               ifmt_ctx1->streams[video_stream_index1]->codecpar->height,
               ifmt_ctx1->streams[video_stream_index1]->codecpar->format,
               ifmt_ctx1->streams[video_stream_index1]->time_base.num, ifmt_ctx1->streams[video_stream_index1]->time_base.den,
               ifmt_ctx1->streams[video_stream_index1]->codecpar->sample_aspect_ratio.num,
               ifmt_ctx1->streams[video_stream_index1]->codecpar->sample_aspect_ratio.den);

       if ((ret = avfilter_graph_create_filter(&buffersrc_ctx1, buffersrc, "in1", args, NULL, filter_graph)) < 0) {
           printf("Cannot create buffer source filter for input file1\n");
           return;
       }

       sprintf(args, "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
               ifmt_ctx2->streams[video_stream_index2]->codecpar->width,
               ifmt_ctx2->streams[video_stream_index2]->codecpar->height,
               ifmt_ctx2->streams[video_stream_index2]->codecpar->format,
               ifmt_ctx2->streams[video_stream_index2]->time_base.num, ifmt_ctx2->streams[video_stream_index2]->time_base.den,
               ifmt_ctx2->streams[video_stream_index2]->codecpar->sample_aspect_ratio.num,
               ifmt_ctx2->streams[video_stream_index2]->codecpar->sample_aspect_ratio.den);

       if ((ret = avfilter_graph_create_filter(&buffersrc_ctx2, buffersrc, "in2", args, NULL, filter_graph)) < 0) {
           printf("Cannot create buffer source filter for input file2\n");
           return;
       }

       // 创建目标滤镜
        const AVFilter *buffersink = avfilter_get_by_name("buffersink");
        enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_YUV420P, AV_PIX_FMT_NONE };
        if ((ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out", NULL, NULL, filter_graph)) < 0) {
            printf("Cannot create buffer sink filter\n");
            return;
        }
    
    
    // 添加滤镜到滤镜图
    
//    AVFilterInOut *outputs2 = avfilter_inout_alloc();
//    outputs2->name = av_strdup("in2");
//    outputs2->filter_ctx = buffersrc_ctx2;
//    outputs2->pad_idx = 0;
//    outputs2->next = NULL;
//
//
//    AVFilterInOut *outputs = avfilter_inout_alloc();
//    outputs->name = av_strdup("in1");
//    outputs->filter_ctx = buffersrc_ctx1;
//    outputs->pad_idx = 0;
//    outputs->next = outputs2;
//
//
//
//    AVFilterInOut *inputs = avfilter_inout_alloc();
//    inputs->name = av_strdup("out");
//    inputs->filter_ctx = buffersink_ctx;
//    inputs->pad_idx = 0;
//    inputs->next = NULL;

//    char filter_descr[256];
//    snprintf(filter_descr, sizeof(filter_descr), "null");
    
    
    const AVFilter *scale = avfilter_get_by_name("scale");
    AVFilterContext *scale_ctx = nil;//avfilter_graph_alloc_filter(filter_graph, overlay, "overlay");
    
//    if ((ret = avfilter_graph_create_filter(&overlay_ctx, overlay, "overlay", NULL, NULL, filter_graph)) < 0) {
//        char errbuf[AV_ERROR_MAX_STRING_SIZE] = {};
//        av_strerror(ret, errbuf, AV_ERROR_MAX_STRING_SIZE);
//        printf("Cannot create filter\n");
//        return;
//    }
//    ret = avfilter_init_str(overlay_ctx, "overlay=x=(W-w)/2:y=(H-h)/2");
//    if (ret < 0) {
//        printf("Cannot create filter\n");
//        return;
//    }

    if ((ret = avfilter_graph_create_filter(&scale_ctx, scale, "scale", "176:h=144:force_original_aspect_ratio=1", NULL, filter_graph)) < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE] = {};
        av_strerror(ret, errbuf, AV_ERROR_MAX_STRING_SIZE);
        printf("Cannot create filter\n");
        return;
    }
    
    const AVFilter *overlay = avfilter_get_by_name("overlay");
    AVFilterContext *overlay_ctx = nil;//avfilter_graph_alloc_filter(filter_graph, overlay, "overlay");
    
//    if ((ret = avfilter_graph_create_filter(&overlay_ctx, overlay, "overlay", NULL, NULL, filter_graph)) < 0) {
//        char errbuf[AV_ERROR_MAX_STRING_SIZE] = {};
//        av_strerror(ret, errbuf, AV_ERROR_MAX_STRING_SIZE);
//        printf("Cannot create filter\n");
//        return;
//    }
//    ret = avfilter_init_str(overlay_ctx, "overlay=x=(W-w)/2:y=(H-h)/2");
//    if (ret < 0) {
//        printf("Cannot create filter\n");
//        return;
//    }

    if ((ret = avfilter_graph_create_filter(&overlay_ctx, overlay, "overlay", "x=10:y=60", NULL, filter_graph)) < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE] = {};
        av_strerror(ret, errbuf, AV_ERROR_MAX_STRING_SIZE);
        printf("Cannot create filter\n");
        return;
    }

//    if ((ret = avfilter_graph_parse_ptr(filter_graph, filter_descr, &inputs, &outputs, NULL)) < 0) {
//        printf("Cannot parse filter graph\n");
//        return;
//    }
    

    
    
    // 连接滤镜
      ret = avfilter_link(buffersrc_ctx1, 0, overlay_ctx, 0);
      if (ret < 0) {
          fprintf(stderr, "Cannot link buffer source1 to overlay\n");
          return;
      }
    
    
     ret = avfilter_link(buffersrc_ctx2, 0, scale_ctx, 0);
    if (ret < 0) {
        fprintf(stderr, "Cannot link buffer source2 to overlay\n");
        return;
    }
      ret = avfilter_link(scale_ctx, 0, overlay_ctx, 1);
      if (ret < 0) {
          fprintf(stderr, "Cannot link buffer source2 to overlay\n");
          return;
      }

      ret = avfilter_link(overlay_ctx, 0, buffersink_ctx, 0);
      if (ret < 0) {
          fprintf(stderr, "Cannot link overlay to buffer sink\n");
          return;
      }

      // 配置滤镜图
      ret = avfilter_graph_config(filter_graph, NULL);
      if (ret < 0) {
          fprintf(stderr, "Cannot configure filter graph\n");
          return;
      }
    
    std::string path = std::string(NSTemporaryDirectory().UTF8String) + "y.yuv";
    FILE* file = fopen(path.c_str(), "wb");
    // 读取帧和应用滤镜
    while (true) {
        BOOL file1Done = NO;
        while (!file1Done) {
            AVPacket* packet1 = av_packet_alloc();
            packet1->data = NULL;
            packet1->size = 0;
            av_init_packet(packet1);
            ret = av_read_frame(ifmt_ctx1, packet1);
            if (ret < 0) {
                file1Done = YES;
                break;
            }
            
            if (packet1->stream_index == video_stream_index1) {
                ret = avcodec_send_packet(codec_ctx1, packet1);
                if (ret < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Error sending a packet to the decoder\n");
                    break;
                }

                BOOL exit1 = NO;
                while (ret >= 0) {
                    AVFrame *frame1 = av_frame_alloc();
                    ret = avcodec_receive_frame(codec_ctx1, frame1);
                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                        //av_frame_free(&frame1);
                        break;
                    }
                    if (ret < 0) {
                        //av_frame_free(&frame1);
                        return;
                    }
                    int width = frame1->width;
                    int height = frame1->height;
                    fwrite(frame1->data[0], 1, width*height, file);
                    fwrite(frame1->data[1], 1, width*height / 4, file);
                    fwrite(frame1->data[2], 1, width*height / 4, file);
                    fflush(file);
                    // 发送帧到输入滤镜1
                    if (av_buffersrc_add_frame_flags(buffersrc_ctx1, frame1, AV_BUFFERSRC_FLAG_PUSH) < 0) {
                        av_log(NULL, AV_LOG_ERROR, "Error while feeding the filtergraph\n");
                    } else {
                        exit1 = YES;
                    }
                    av_frame_free(&frame1);
            
                    
                    break;
                }
                if(exit1) {
                    break;
                }
            }
            
            av_packet_free(&packet1);
        }
        
        BOOL file2Done = NO;
        while (!file2Done) {
            
            AVPacket* packet2 = av_packet_alloc();
            packet2->data = NULL;
            packet2->size = 0;
            av_init_packet(packet2);
            ret = av_read_frame(ifmt_ctx2, packet2);
            if (ret < 0) {
                file2Done = YES;
                break;
            }
            
            if (packet2->stream_index == video_stream_index2) {
                ret = avcodec_send_packet(codec_ctx2, packet2);
                if (ret < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Error sending a packet to the decoder\n");
                    break;
                }
                
                BOOL exit2 = NO;
                while (ret >= 0) {
                    AVFrame *frame2 = av_frame_alloc();
                    ret = avcodec_receive_frame(codec_ctx2, frame2);
                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                        //av_frame_free(&frame1);
                        break;
                    }
                    if (ret < 0) {
                        //av_frame_free(&frame1);
                        break;
                    }
                    
                    // 发送帧到输入滤镜1
                    if (av_buffersrc_add_frame_flags(buffersrc_ctx2, frame2, AV_BUFFERSRC_FLAG_PUSH) < 0) {
                        av_log(NULL, AV_LOG_ERROR, "Error while feeding the filtergraph\n");
     
                    } else {
                        exit2 = YES;
                    }
                    av_frame_free(&frame2);
                    
                }
                if(exit2) {
                    break;
                }
            }
            av_packet_free(&packet2);
        }
        
        
        // 从滤镜图形中获取处理后的帧
        
        BOOL exit = NO;
    while (!exit) {
        
        if (file1Done && file2Done) {
            break;
        }
        AVFrame *filterFrame = av_frame_alloc();
        ret = av_buffersink_get_frame_flags(buffersink_ctx, filterFrame, AV_BUFFERSRC_FLAG_KEEP_REF);//av_buffersink_get_frame(buffersink_ctx, filterFrame);
        if (ret == AVERROR_EOF) {
            break;;
        }
        
        if (ret == AVERROR(EAGAIN)) {
            break;
        }
        if (ret < 0) {
            break;
        }
        filterFrame->pts = av_rescale_q(filterFrame->pts, buffersink_ctx->inputs[0]->time_base, out_stream->time_base);
        filterFrame->pkt_dts = av_rescale_q(filterFrame->pkt_dts, buffersink_ctx->inputs[0]->time_base, out_stream->time_base);
        filterFrame->pkt_duration = av_rescale_q(filterFrame->pkt_duration, buffersink_ctx->inputs[0]->time_base, out_stream->time_base);
        
        ret = avcodec_send_frame(codec_ctx_out, filterFrame);
        if (ret < 0) {
            fprintf(stderr, "Error sending frame to codec: %s\n", av_err2str(ret));
        }
       while (ret >= 0) {
           AVPacket* pkt = av_packet_alloc();
            av_init_packet(pkt);
            pkt->data = NULL;
           pkt->size = 0;
           ret = avcodec_receive_packet(codec_ctx_out, pkt);
           if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
               break;
           }
           if (ret < 0) {
               fprintf(stderr, "Error receiving packet from codec: %s\n", av_err2str(ret));
               return;
           }

           // 写入帧
           
//                   av_packet_rescale_ts(&pkt, codec_ctx_out->time_base, out_stream->time_base);
//                   pkt.stream_index = out_stream->index;
           ret = av_interleaved_write_frame(ofmt_ctx, pkt);
           if (ret < 0) {
               fprintf(stderr, "Error writing frame to file: %s\n", av_err2str(ret));
               return;
           }
           av_interleaved_write_frame(ofmt_ctx, nullptr);
           av_packet_free(&pkt);
       }
        av_frame_free(&filterFrame);
    }

        if (file1Done && file2Done) {
            av_write_trailer(ofmt_ctx);
            break;
        }
        
        

    }
    
      
    if (codec_ctx1) {
        avcodec_close(codec_ctx1);
    }
    
    if (codec_ctx2) {
        avcodec_close(codec_ctx2);
    }
    
    if (codec_ctx_out) {
        avcodec_close(codec_ctx_out);
    }
    
    if (ifmt_ctx1) {
        avformat_close_input(&ifmt_ctx1);
    }

    if (ifmt_ctx2) {
        avformat_close_input(&ifmt_ctx2);
    }

    if (ofmt_ctx_codec) {
        avcodec_free_context(&ofmt_ctx_codec);
    }

    if (ofmt_ctx) {
        avformat_free_context(ofmt_ctx);
    }

    if (buffersrc_ctx1) {
        avfilter_free(buffersrc_ctx1);
    }
    
    if (buffersrc_ctx2) {
        avfilter_free(buffersrc_ctx2);
    }

    if (buffersink_ctx) {
        avfilter_free(buffersink_ctx);
    }
    
    if (scale_ctx) {
        avfilter_free(scale_ctx);
    }
    
    if (overlay_ctx) {
        avfilter_free(overlay_ctx);
    }

    if (filter_graph) {
        avfilter_graph_free(&filter_graph);
    }

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
            const AVCodec* codec = avcodec_find_decoder(videoCodeParameter->codec_id);
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
            const AVCodec* codec = avcodec_find_decoder(audioCodeParameter->codec_id);
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
            if(avcodec_receive_frame(videoCodeCtx, videoFrame) == 0) {
                NSLog(@"video width %d, height %d",videoFrame->width,videoFrame->height);
                if (videoCodeCtx->pix_fmt == AV_PIX_FMT_YUV420P || videoCodeCtx->pix_fmt == AV_PIX_FMT_YUVJ420P) {
                    [self saveToYUVFile:videoFrame codecCtx:videoCodeCtx];
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
                //NSLog(@"audio width %d, height %d",audioFrame->width,audioFrame->height);
                [self saveToPCMFile:audioFrame codecCtx:audioCodecCtx];
            } else {
                NSLog(@"audio decode failed %d",result);
            }
            av_frame_free(&audioFrame);
        }
    }
    
    avcodec_free_context(&videoCodeCtx);
    avcodec_free_context(&audioCodecCtx);
    avformat_free_context(formatContext);
    [self.videoOutput close];
    [self.audioOutput close];
    NSString* targetPath = [self.audioPath stringByDeletingLastPathComponent];
    NSString* targetName = [self.audioPath.lastPathComponent stringByDeletingPathExtension];
    targetPath = [targetPath stringByAppendingFormat:@"/%@.wav",targetName];
    pcmf32le_to_wave(self.audioPath.UTF8String,1,0,targetPath.UTF8String);
    
}

- (void)saveToYUVFile:(AVFrame*)frame codecCtx:(AVCodecContext*)ctx {
    /// ffplay -f rawvideo -video_size 1280x960 1532872056009.yuv
    int i = 0;
    int width = ctx->width;
    int height = ctx->height;
    int halfWidth = width / 2;
    int halfHeight = height / 2;
    int yWrap = frame->linesize[0];
    int uWrap = frame->linesize[1];
    int vWrap = frame->linesize[2];
    
    uint8_t* yBuf = frame->data[0];
    uint8_t* uBuf = frame->data[1];
    uint8_t* vBuf = frame->data[2];
    for(i = 0; i < height; i++) {
        [self.videoOutput write:yBuf + i * yWrap maxLength:width];
    }
    
    for(i = 0; i < halfHeight; i++) {
        [self.videoOutput write:uBuf + i * uWrap maxLength:halfWidth];
    }
    
    for (i = 0; i < halfHeight; i++) {
        [self.videoOutput write:vBuf + i * vWrap maxLength:halfWidth];
    }
}

- (void)saveToPCMFile:(AVFrame*)frame codecCtx:(AVCodecContext*)ctx  {
    
    ///ffplay 1532876154300.pcm -f f32le -channels 1 -ar 44100
    /// -f 表示格式 -channels 表示声道数 -ar 表示采样率
//    if (av_sample_fmt_is_planar(frame->format)) {
//         int data_size = av_get_bytes_per_sample(ctx->sample_fmt);
//        for(int i = 0 ;i < frame->nb_samples; i ++) {
//            for (int ch = 0; ch < ctx->channels; ch ++) {
//                [self.audioOutput write:frame->data[ch] + data_size * i maxLength:data_size];
//            }
//        }
//    } else {
//        int data_size = av_samples_get_buffer_size(NULL, frame->channels, frame->nb_samples, frame->format, 0);
//        //    if (data_size < 0) {
//        //        NSLog(@"save to pcm file av_samples_get_buffer_size %ld",data_size);
//        //        return;
//        //    }
//        [self.audioOutput write:frame->data[0] maxLength:data_size];
//    }
}


int pcmf32le_to_wave(const char *pcmpath, int channels, int sample_rate, const char *wavepath)
{
    typedef struct WAVE_HEADER{
        char    fccID[4];       //内容为""RIFF
        unsigned long dwSize;   //最后填写，WAVE格式音频的大小
        char    fccType[4];     //内容为"WAVE"
    }WAVE_HEADER;
    
    typedef struct WAVE_FMT{
        char    fccID[4];          //内容为"fmt "
        unsigned long  dwSize;     //内容为WAVE_FMT占的字节数，为16
        unsigned short wFormatTag; //如果为PCM，改值为 1
        unsigned short wChannels;  //通道数，单通道=1，双通道=2
        unsigned long  dwSamplesPerSec;//采用频率
        unsigned long  dwAvgBytesPerSec;/* ==dwSamplesPerSec*wChannels*uiBitsPerSample/8 */
        unsigned short wBlockAlign;//==wChannels*uiBitsPerSample/8
        unsigned short uiBitsPerSample;//每个采样点的bit数，8bits=8, 16bits=16
    }WAVE_FMT;
    
    typedef struct WAVE_DATA{
        char    fccID[4];       //内容为"data"
        unsigned long dwSize;   //==NumSamples*wChannels*uiBitsPerSample/8
    }WAVE_DATA;
    
    if(channels==2 || sample_rate==0)
    {
        channels = 2;
        sample_rate = 44100;
    }
    int bits = 32;
    
    WAVE_HEADER pcmHEADER;
    WAVE_FMT    pcmFMT;
    WAVE_DATA   pcmDATA;
    
    unsigned short m_pcmData;
    FILE *fp, *fpout;
    
    fp = fopen(pcmpath, "rb+");
    if(fp==NULL)
    {
        printf("Open pcm file error.\n");
        return -1;
    }
    fpout = fopen(wavepath, "wb+");
    if(fpout==NULL)
    {
        printf("Create wav file error.\n");
        return -1;
    }
    
    /* WAVE_HEADER */
    memcpy(pcmHEADER.fccID, "RIFF", strlen("RIFF"));
    memcpy(pcmHEADER.fccType, "WAVE", strlen("WAVE"));
    fseek(fpout, sizeof(WAVE_HEADER), 1);   //1=SEEK_CUR
    /* WAVE_FMT */
    memcpy(pcmFMT.fccID, "fmt ", strlen("fmt "));
    pcmFMT.dwSize = 16;
    pcmFMT.wFormatTag = 1;
    pcmFMT.wChannels = channels;
    pcmFMT.dwSamplesPerSec = sample_rate;
    pcmFMT.uiBitsPerSample = bits;
    /* ==dwSamplesPerSec*wChannels*uiBitsPerSample/8 */
    pcmFMT.dwAvgBytesPerSec = pcmFMT.dwSamplesPerSec*pcmFMT.wChannels*pcmFMT.uiBitsPerSample/8;
    /* ==wChannels*uiBitsPerSample/8 */
    pcmFMT.wBlockAlign = pcmFMT.wChannels*pcmFMT.uiBitsPerSample/8;
    
    
    fwrite(&pcmFMT, sizeof(WAVE_FMT), 1, fpout);
    
    /* WAVE_DATA */
    memcpy(pcmDATA.fccID, "data", strlen("data"));
    pcmDATA.dwSize = 0;
    fseek(fpout, sizeof(WAVE_DATA), SEEK_CUR);
    
    fread(&m_pcmData, sizeof(unsigned short), 1, fp);
    while(!feof(fp))
    {
        pcmDATA.dwSize += 2;
        fwrite(&m_pcmData, sizeof(unsigned short), 1, fpout);
        fread(&m_pcmData, sizeof(unsigned short), 1, fp);
    }
    
    /*pcmHEADER.dwSize = 44 + pcmDATA.dwSize;*/
    //修改时间：2018年1月5日
    pcmHEADER.dwSize = 36 + pcmDATA.dwSize;
    
    rewind(fpout);
    fwrite(&pcmHEADER, sizeof(WAVE_HEADER), 1, fpout);
    fseek(fpout, sizeof(WAVE_FMT), SEEK_CUR);
    fwrite(&pcmDATA, sizeof(WAVE_DATA), 1, fpout);
    
    fclose(fp);
    fclose(fpout);
    
    return 0;
}



@end
