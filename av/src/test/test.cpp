#include "test.h"
#include<iostream>

Test::Test() {
	std::cout << "test" << std::endl;	
}

Test::~Test() {
  std::cout << "~ test" << std::endl;
}
