//
//  FileInput.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/10.
//

#ifndef FileInput_hpp
#define FileInput_hpp

#include "Linkable.hpp"
#include "CIImageReader.hpp"

class CIFileInput : public Linkable <CIImage>{
private:
    CIImageReader* reader;
  
public:
    
    CIFileInput(CIImageReader& reader) {
        this->reader = &reader;
    }
    
    virtual int getNumInputs() const override {
        return 0;
    }
    
    virtual int getNumOutputs() const override {
        return 1;
    }
    
    
    
    virtual void setOutput(int index, Linkable<CIImage>& filter, int port) override {
        
    }
    
    virtual CIImage* getFrame(int index = 0) const override {
        return reader->read();
    }
};

#endif /* FileInput_hpp */
