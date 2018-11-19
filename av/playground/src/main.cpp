//
//  main.cpp
//  PlayGround
//
//  Created by birney on 2018/11/19.
//  Copyright © 2018年 Grocery. All rights reserved.
//

#include <stdio.h>
#include "test.h"

int main(int argc, char* argv[]) {
    Test t = Test();
    //t.testFileStream();
    t.testLambda();
    t.testSmartPoint();
    
    return 0;
}
