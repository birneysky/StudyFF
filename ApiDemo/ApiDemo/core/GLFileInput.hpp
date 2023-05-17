//
//  GLFileInput.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/15.
//

#ifndef GLFileInput_hpp
#define GLFileInput_hpp

#include "Linkable.hpp"
#include "GLTextureReader.hpp"
#include "GLTextureFrame.hpp"

class GLFileInput: public Linkable <GLTextureFrame>{
private:
    GLTextureReader* reader;
public:
    GLFileInput(GLTextureReader& reader) {
        this->reader = &reader;
    }
    
    virtual int getNumInputs() const override {
        return 0;
    }
    
    virtual int getNumOutputs() const override {
        return 1;
    }
    
    
    
    virtual void setOutput(int index, Linkable<GLTextureFrame>& filter, int port) override {
        
    }
    
    virtual GLTextureFrame* getFrame(int index = 0) const override {
        return reader->read();
    }
    //GLTextureReader
};

#endif /* GLFileInput_hpp */
