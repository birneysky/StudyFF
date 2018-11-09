#include <stdlib.h>
#include "av_file.h"
#include <fstream>
extern "C" {
  #include <libavutil/log.h>
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

  std::ofstream audioFile(audio_file_name);
  AVPacket pkt;
  av_init_packet(&pkt);
  while(av_read_frame(fmt_ctx, &pkt) >= 0){
    if(pkt.stream_index == audio_index) {
      /// https://blog.csdn.net/sz76211822/article/details/53693018
      ///http://www.mworkbox.com/wp/work/642.html
      /// https://www.cnblogs.com/bigrabbit/archive/2012/09/20/2695543.html
      /// https://blog.csdn.net/xiaolewennofollow/article/details/51172247
      audioFile.write((const char*)pkt.data, pkt.size);
      //fwrite(pkt.data, 1, pkt.size, nullptr);

    }
  }
  audioFile.close();

  avformat_close_input(&fmt_ctx);
  return 0;
}
