//
//  main.cpp
//  PlayGround
//
//  Created by birney on 2018/11/19.
//  Copyright © 2018年 Grocery. All rights reserved.
//

#include <stdio.h>
#include "test.h"
#include <thread>
#include <chrono>
#include <iostream>



int main(int argc, char* argv[]) {
    Test t = Test();
    //t.testFileStream();
    t.testLambda();
    t.testSmartPoint();
    t.testTypeId();
    t.testThread();
    t.testThread2();
    t.testThread3();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Hello C++ 11" << std::endl;
    t.testStdFunction();
    t.testStdforeach();
    return EXIT_SUCCESS;
}
