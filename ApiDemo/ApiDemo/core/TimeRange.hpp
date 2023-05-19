//
//  TimeRange.hpp
//  ApiDemo
//
//  Created by Bruce on 2023/5/18.
//  Copyright © 2023 Grocery. All rights reserved.
//

#ifndef TimeRange_hpp
#define TimeRange_hpp

struct TimeRanage{
private:
    float start;
    float duration;
public:
    
    TimeRanage(const float& start, const float& duration):start(start),duration(duration) {
    }
    
    float start() const {
        return start;
    }
    
    float duration() const {
        return duration
    }
    
    float end() const {
        return start + duration;
    }
}

#endif /* TimeRange_hpp */
