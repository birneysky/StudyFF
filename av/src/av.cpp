#include <iostream>
#include "av_file.h"
extern "C"{
	#include <libavutil/log.h>
  #include <libavformat/avformat.h>
}
using namespace std;

int main( int argc, char *argv[] ) {
  avformat_network_init();
  av_log_set_level(AV_LOG_DEBUG);
	av_log(NULL, AV_LOG_INFO, "%s\n", "Hello AV World");
  file_test();
  print_media_info("");
  avformat_network_deinit();
	 return 0;
}
