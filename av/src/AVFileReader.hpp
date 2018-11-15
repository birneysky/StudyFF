//
//  AVFileReader.hpp
//  AV
//
//  Created by birney on 2018/11/15.
//  Copyright © 2018年 Grocery. All rights reserved.
//

#ifndef AVFileReader_hpp
#define AVFileReader_hpp

#include <stdio.h>
#include <string>

class AVFileReader {
public:
    AVFileReader(const std::string& path);
    ~AVFileReader();
    bool startReading();
    bool stopReading();
};

#endif /* AVFileReader_hpp */
