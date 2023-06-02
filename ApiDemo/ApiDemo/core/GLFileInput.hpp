//
//  GLFileInput.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/15.
//

#ifndef GLFileInput_hpp
#define GLFileInput_hpp

#include "Linkable.hpp"
#include "AssetReader.hpp"
#include "GLTextureReader.hpp"
#include "GLTextureFrame.hpp"

class GLFileInput: public Linkable <GLTextureFrame>{
private:
    AssetReader* reader;
    //GLTextureReader* _reader;
public:
    GLFileInput(AssetReader& reader) {
        this->reader = &reader;
        //_reader = &reader;
    }
    
    ~GLFileInput() {
        std::cout << "~GLFileInput() " << this << std::endl;
    }
    
    
    int getNumInputs() const override {
        return 0;
    }
    
    int getNumOutputs() const override {
        return 1;
    }
    
    
    
    void setOutput(int index, Linkable<GLTextureFrame>& filter, int port) override {
        
    }
    
    virtual GLTextureFrame* getFrame(int index = 0) override {
        //return _reader->read();
        if (reader->status() == AssetReader::Status::unknown) {
            reader->startReading();
        }
        return reader->getNextFrame(AssetReader::MediaType::Video);
    }
    //GLTextureReader
};

#endif /* GLFileInput_hpp */
