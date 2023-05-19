//
//  AssetReadeer.hpp
//  ApiDemo
//
//  Created by Bruce on 2023/5/18.
//  Copyright © 2023 Grocery. All rights reserved.
//

#ifndef AssetReadeer_hpp
#define AssetReadeer_hpp

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

//case unknown
//The asset reader is in an unknown state.
//case reading
//The asset reader is successfully reading samples from its asset.
//case completed
//The asset reader completes reading all samples within its specified time range.
//case failed
//The asset reader can no longer read samples from its asset because of an error.
//case cancelled

template<typename T>
class AssetReaderOutput {
public:
    enum class MediaType {Video, Audio};
    AssetReaderOutput(MediaType mediaType) {
        
    }
    
    virtual T* readNextFrame() = 0;
    
};

class CiimageReaderOutput: public AssetReaderOutput<CIImage> {
    CiimageReaderOutput():AssetReaderOutput(MediaType::Video) {}
    
    CIImage* readNextFrame() override {
        return nullptr;
    }
};


class AssetReader {
public:
    enum class Status {unknown, reading, completed, failed, cancelled};
private:
    
    AVFormatContext *ifmt_ctx = nullptr;
    std::string path;
    Status _status;
    
public:
    
    AssetReader(const std::string& filePath) {
        ifmt_ctx = avformat_alloc_context();
        path = filePath;
        _status = Status::unknown;
    }
    
    Status status()  const {
        return _status;
    }
    
    bool startReading() {
        int ret = avformat_open_input(&ifmt_ctx, path.c_str(), NULL, NULL);
        if (ret  < 0) {
            std::cout << "cannot open input file" << path <<std::endl;
            return false;
        }
        
        ret = avformat_find_stream_info(ifmt_ctx, NULL);
        if (ret < 0) {
            std::cout << "cannot find input file stream information. file location:" << path << std::endl;
            return false;
        }
        _status = Status::reading;
        return true;
    }
    
};

#endif /* AssetReadeer_hpp */
