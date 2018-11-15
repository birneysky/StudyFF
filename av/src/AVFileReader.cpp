//
//  AVFileReader.cpp
//  AV
//
//  Created by birney on 2018/11/15.
//  Copyright © 2018年 Grocery. All rights reserved.
//

#include "AVFileReader.hpp"
#include <iostream>


AVFileReader::AVFileReader(const std::string& path) {
//    if (path.length() <= 0) {
//        throw std::exception();
//    }
    std::cout << "AVFileReader" << std::endl;
}
AVFileReader::~AVFileReader() {
    std::cout << "~AVFileReader" << std::endl;
}
