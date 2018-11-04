#include <iostream>
#include "add.h"
extern "C"{
	#include <libavutil/log.h>
}
using namespace std;

int main( int argc, char *argv[] ) {
	av_log_set_level(AV_LOG_DEBUG);
	cout << "hell world " << add(3,4)  <<endl;
	av_log(NULL, AV_LOG_INFO, "%s\n", "483204");
	 return 0;
}
