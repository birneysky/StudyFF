#include "test.h"
#include<iostream>
#include <fstream>

Test::Test() {
	std::cout << "test" << std::endl;	
}

Test::~Test() {
 	std::cout << "~ test" << std::endl;
}

/// c++ 写文件操作
void Test::testFileStream() {
	std::ofstream test_file("./ttt.txt");
    test_file << "1234567" << std::endl;
    test_file << "48320483209" << std::endl;
    test_file << 2 << 5 << 3.08 << 3.9;
    const char buf[6] = {'1','b','d','e','f','g'};
    for(int i = 0; i < 10; i++) {
        test_file.write(buf,6);
    }
    test_file.close();
}


/// shared_ptr 引用计数操作
void Test::testSmartPoint() {

    //std::string file_full_name(argv[1]);
    //avformat_network_init();

    //file_test();
    //print_media_info(fileName);
    //extra_audio_data(file_full_name);
    //avformat_network_deinit();

    std::shared_ptr<Test> p1 = std::make_shared<Test>();
    std::cout <<  "1 ref:" << p1.use_count() << std::endl;
    {
        std::shared_ptr<Test> p2 = p1;
        std::cout << "2 ref:" << p1.use_count() <<std::endl;
    }
    std::cout << "3 ref:" << p1.use_count() << std::endl;
}
