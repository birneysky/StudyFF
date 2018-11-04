#include <stdlib.h>
#include "av_file.h"
extern "C" {
  #include <libavutil/log.h>
  #include <libavformat/avformat.h>
}

int file_test () {
  int ret = avpriv_io_delete("./myteestfile.txt");
  if (ret < 0) {
    av_log(NULL,AV_LOG_ERROR,"Failed to delete file mytestfile.txt\n");
    return ret;
  }
   av_log(NULL, AV_LOG_INFO, "Success to delete mytestfile.txt\n");
  ret = avpriv_io_move("111.txt", "222.txt");
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Failed to rename\n");
    return ret;
  }
  av_log(NULL, AV_LOG_INFO, "Success to rename\n");
  return 0;
}

int  print_media_info(const std::string& fileName) {
  AVFormatContext* fmt_ctx = NULL;
  int ret = avformat_open_input(&fmt_ctx, fileName.c_str(),NULL,NULL); 
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR,"can't open file: %s\n",av_err2str(ret));
    return -1;
  }

  av_dump_format(fmt_ctx, 0, fileName.c_str(),0);
  avformat_close_input(&fmt_ctx);
  return 0;
}
