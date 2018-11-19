//
//  AVFileReader.hpp
//  AV
//
//  Created by birney on 2018/11/15.
//  Copyright © 2018年 Grocery. All rights reserved.
//

#ifndef AVFileReader_hpp
#define AVFileReader_hpp

#include <cstdio>
#include <string>

extern "C"{
    #include <libavutil/log.h>
    #include <libavformat/avformat.h>
}

class AVFileReader {
private:
    std::string localPath;
    AVFormatContext* formatCtx;
public:
    AVFileReader(const std::string& path);
    ~AVFileReader();
    bool startReading();
    bool stopReading();
    AVStream* getStream(int streamIndex);
    unsigned int getStreamCount();
    AVPacket* readNextFrame();
};

#endif /* AVFileReader_hpp */
