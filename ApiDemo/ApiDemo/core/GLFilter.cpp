//
//  GLFilter.cpp
//  ApiDemo
//
//  Created by Bruce on 2023/5/19.
//  Copyright © 2023 Grocery. All rights reserved.
//

#include "GLFilter.hpp"


const std::string GLFilter::vertexShader = R"(
             attribute vec4 position;
             attribute vec4 texcoord;

             varying vec2 textureCoordinate;

             void main()
             {
                 gl_Position = position;
                 textureCoordinate = texcoord.xy;
             }
)";