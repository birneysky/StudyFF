#include <iostream>
#include "av_file.h"
extern "C"{
	#include <libavutil/log.h>
  #include <libavformat/avformat.h>
}
using namespace std;

int main( int argc, char *argv[] ) {
  av_log_set_level(AV_LOG_INFO);
  if (argc < 2 ){
    av_log(nullptr,AV_LOG_ERROR, "argc less than 2 \n");
    return -1;
  }
  std::string fileName(argv[1]);  
  avformat_network_init();
	av_log(NULL, AV_LOG_INFO, "%s\n", "Hello AV World");
  //file_test();
  //print_media_info(fileName);
  extra_audio_data(fileName);
  avformat_network_deinit();
	 return 0;
}
