//
//  Reader.hpp
//  AVEditorApp
//
//  Created by Bruce on 2023/5/15.
//

#ifndef Reader_hpp
#define Reader_hpp

template<typename T>
class Reader {
public:
    virtual T* read() = 0;
};

#endif /* Reader_hpp */
