//
//  Linkable.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/10.
//

#ifndef Linkable_hpp
#define Linkable_hpp
#include <map>

template<typename T>
class Linkable {
public:
    typedef struct Link{
        Link():target(nullptr),port(-1) {}
        Link(Linkable* target, int port):target(target),port(port) {}
        Linkable* target;
        int port;
    } Link;
    
private:
    std::map<int, Link> inputs;
public:
    virtual ~Linkable() = default;
    virtual int getNumInputs() const = 0;
    virtual int getNumOutputs() const = 0;

    virtual T* getFrame(int index = 0) const = 0;
    void connect(Linkable& dst, int dst_port, int src_port = 0) {
        dst.setInput(dst_port, *this, src_port);
    };
    Link getInputLink(int port) const {
        auto a = inputs.find(port);
        return a->second;
    }
    void setInput(int index, Linkable& dst, int port) {
        Link link{&dst, port};
        inputs.insert(std::make_pair(index, link));
    };
    virtual void setOutput(int index, Linkable& dst, int port) = 0;
    //virtual TimeRange effectiveTime() const = 0;
};






#endif /* Linkable_hpp */
