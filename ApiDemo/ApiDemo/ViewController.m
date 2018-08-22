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
    if (av_sample_fmt_is_planar(frame->format)) {
         int data_size = av_get_bytes_per_sample(ctx->sample_fmt);
        for(int i = 0 ;i < frame->nb_samples; i ++) {
            for (int ch = 0; ch < ctx->channels; ch ++) {
                [self.audioOutput write:frame->data[ch] + data_size * i maxLength:data_size];
            }
        }
    } else {
        int data_size = av_samples_get_buffer_size(NULL, frame->channels, frame->nb_samples, frame->format, 0);
        //    if (data_size < 0) {
        //        NSLog(@"save to pcm file av_samples_get_buffer_size %ld",data_size);
        //        return;
        //    }
        [self.audioOutput write:frame->data[0] maxLength:data_size];
    }
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
