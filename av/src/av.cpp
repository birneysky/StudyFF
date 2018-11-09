#include <iostream>
#include "av_file.h"
#include "test/test.h"

extern "C"{
	#include <libavutil/log.h>
  #include <libavformat/avformat.h>
}
//using namespace std;

int main( int argc, char *argv[] ) {

  av_log_set_level(AV_LOG_INFO);
  av_log(NULL, AV_LOG_INFO, "%s\n", "Hello AV World");
  if (argc < 2 ){
    av_log(nullptr,AV_LOG_ERROR, "argc less than 2 \n");
    return -1;
  }
  std::string file_full_name(argv[1]);  
  //avformat_network_init();
  
  //file_test();
  //print_media_info(fileName);
  extra_audio_data(file_full_name);
  //avformat_network_deinit();

  std::shared_ptr<Test> p1 = std::make_shared<Test>();
  std::cout <<  "1 ref:" << p1.use_count() << std::endl;
  {
    std::shared_ptr<Test> p2 = p1; 
    std::cout << "2 ref:" << p1.use_count() <<std::endl;
  }
  std::cout << "3 ref:" << p1.use_count() << std::endl; 
  p1->testFileStream();
  return 0;
}
