#include "test.h"
#include<iostream>
#include <fstream>

Test::Test() {
	std::cout << "test" << std::endl;	
}

Test::~Test() {
 	std::cout << "~ test" << std::endl;
}

void Test::testFileStream() {
	std::ofstream test_file("./ttt.txt");
  	test_file << "1234567" << std::endl;
  	test_file << "48320483209" << std::endl;
  	test_file << 2 << 5 << 3.08 << 3.9;

 	test_file.close();
}
