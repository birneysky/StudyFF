#include <stdlib.h>
#include "av_file.h"
#include <fstream>
extern "C" {
  #include <libavutil/log.h>
  #include <libavutil/mathematics.h>
  #include <libavformat/avio.h>
  #include <libavformat/avformat.h>
}

int file_test () {
  int ret = avpriv_io_delete("./myteestfile.txt");
  if (ret < 0) {
    av_log(nullptr,AV_LOG_ERROR,"Failed to delete file mytestfile.txt\n");
    return ret;
  }
   av_log(nullptr, AV_LOG_INFO, "Success to delete mytestfile.txt\n");
  ret = avpriv_io_move("111.txt", "222.txt");
  if (ret < 0) {
    av_log(nullptr, AV_LOG_ERROR, "Failed to rename\n");
    return ret;
  }
  av_log(nullptr, AV_LOG_INFO, "Success to rename\n");
  return 0;
}

int  print_media_info(const std::string& fileName) {
  AVFormatContext* fmt_ctx = nullptr;
  int ret = avformat_open_input(&fmt_ctx, fileName.c_str(),nullptr,nullptr); 
  if (ret < 0) {
    av_log(nullptr, AV_LOG_ERROR,"can't open file: %s\n",av_err2str(ret));
    return -1;
  }

  av_dump_format(fmt_ctx, 0, fileName.c_str(),0);
  avformat_close_input(&fmt_ctx);
  return 0;
}


int extra_audio_data(const std::string& file_name) {
  /// av_init_packet() av_packet_unref()   初始化一个数据包结构体
  /// av_find_best_stream()  在多媒体文件中找到最好的一路流
  // av_read_frame  读取数据包
  if (file_name.length() < 0) {
    av_log(nullptr, AV_LOG_ERROR,"file_name is invalid");
    return -1;
  }
  AVFormatContext* fmt_ctx = nullptr;
  int ret = avformat_open_input(&fmt_ctx, file_name.c_str(),nullptr,nullptr); 
  if (ret < 0) {
    av_log(nullptr, AV_LOG_ERROR,"can't open file: %s\n",av_err2str(ret));
    return -1;
  }
  
  av_dump_format(fmt_ctx, 0, file_name.c_str(),0);
  ret = av_find_best_stream(fmt_ctx,AVMEDIA_TYPE_AUDIO,-1,-1,nullptr,0);
  if (ret < 0) {
    av_log(nullptr, AV_LOG_ERROR,"can't open file: %s\n",av_err2str(ret));
    avformat_close_input(&fmt_ctx);
    return -1;
  }
  av_dump_format(fmt_ctx, 0, file_name.c_str(),0);
  int audio_index = av_find_best_stream(fmt_ctx,AVMEDIA_TYPE_AUDIO,-1,-1,nullptr,0);
  
  if (audio_index < 0) {
    av_log(nullptr, AV_LOG_ERROR,"can't find the best stream %s\n",av_err2str(ret));
    avformat_close_input(&fmt_ctx);
    return -1;
  }
  AVStream* audio_stream = fmt_ctx->streams[audio_index];
  const char* codec_name = avcodec_get_name(audio_stream->codecpar->codec_id);

  std::string audio_file_name = std::string(file_name);
  std::size_t pos = audio_file_name.find_last_of("/");
  if (pos != std::string::npos) { 
    audio_file_name = audio_file_name.substr(pos+1);
  } 
  pos = audio_file_name.find_last_of(".");
  audio_file_name = audio_file_name.substr(0,pos+1) + codec_name;

  av_log(nullptr,AV_LOG_INFO, "audio file name: %s\n",audio_file_name.c_str());
  AVOutputFormat* output_fmt = av_guess_format(nullptr,audio_file_name.c_str(),nullptr);
  if(!output_fmt) {
    av_log(nullptr,AV_LOG_ERROR,"could not guss file format");
    avformat_close_input(&fmt_ctx);
    return -1;
  }
  AVFormatContext* ofmt_ctx = avformat_alloc_context();
  ofmt_ctx->oformat = output_fmt;
  AVStream* out_stream = avformat_new_stream(ofmt_ctx,nullptr);
 
  int err_code = avcodec_parameters_copy(out_stream->codecpar, audio_stream->codecpar);
  if(err_code < 0) {
    char errors[1024]; 
    av_strerror(err_code, errors, 1024);
     av_log(NULL, AV_LOG_ERROR, "Could not copy file parameters %s, %d(%s)\n",
               audio_file_name.c_str(),
               err_code,
               errors);
  }

  out_stream->codecpar->codec_tag = 0;
  err_code = avio_open(&ofmt_ctx->pb, audio_file_name.c_str(), AVIO_FLAG_WRITE);
  if(err_code < 0) {
    
  }

  if(avformat_write_header(ofmt_ctx,nullptr) < 0 ) {
    av_log(NULL, AV_LOG_DEBUG, "Error occurred when opening output file");
    return -1;
  }
  AVPacket pkt;
  av_init_packet(&pkt);
  while(av_read_frame(fmt_ctx, &pkt) >= 0){
    if(pkt.stream_index == audio_index) {
      pkt.pts = av_rescale_q_rnd(pkt.pts, audio_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
      pkt.dts = pkt.pts;
      pkt.duration = av_rescale_q(pkt.duration, audio_stream->time_base, out_stream->time_base);
      pkt.pos = -1;
      pkt.stream_index = 0;
      av_interleaved_write_frame(ofmt_ctx, &pkt);
      av_packet_unref(&pkt);
    }
  }

  av_write_trailer(ofmt_ctx); 
  avformat_close_input(&fmt_ctx);
  return 0;
}
