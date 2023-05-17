//
//  Filter.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/5.
//

#ifndef Filter_hpp
#define Filter_hpp

#include <string>
#include "Linkable.hpp"

template<typename T>
class Filter : public Linkable<T> {
public:
    virtual ~Filter() {
        
    }
    
    Filter(const std::string& name) {
        this->name = name;
    }
    
    virtual int getNumInputs() const override {
        return 1;
    }
    virtual int getNumOutputs() const override {
        return 1;
    }
    
    
    
    virtual void setOutput(int index, Linkable<T>& dst, int port) override{
        
    }
    
    virtual T* getFrame(int index = 0) const  override {
        return nullptr;
    }
    
private:
    std::string name;
    Linkable<T>* dst;
    int dst_port;
};


#endif /* Filter_hpp */
