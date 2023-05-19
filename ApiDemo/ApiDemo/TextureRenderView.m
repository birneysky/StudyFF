//
//  TextureRender.m
//  OpenGLRenderTexture
//
//  Created by Bruce on 2023/5/16.
//

#import "TextureRenderView.h"
#import <AVFoundation/AVFoundation.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#define STRINGIZE(x) #x
#define STRINGIZE2(x) STRINGIZE(x)
#define SHADER_STRING(text) @ STRINGIZE2(text)

NSString *const vertexShaderString = SHADER_STRING
(
 attribute vec4 position;
 attribute vec2 texcoord;
 varying vec2 v_texcoord;
 
 void main()
 {
     gl_Position = position;
     v_texcoord = texcoord.xy;
 }
);

NSString *const rgbFragmentShaderString = SHADER_STRING
(
 varying highp vec2 v_texcoord;
 uniform sampler2D inputImageTexture;
 
 void main()
 {
     gl_FragColor = texture2D(inputImageTexture, v_texcoord);
 }
);



@implementation TextureRenderView {
    EAGLContext* _context;
    GLuint                                  _framebuffer;
    GLuint                                  _renderbuffer;
    GLint                                   _backingWidth;
    GLint                                   _backingHeight;
    GLuint _filterProgram;
    
    GLint                               filterPositionAttribute;
    GLint                               filterTextureCoordinateAttribute;
    GLint                               filterInputTextureUniform;
    
    NSInteger                           frameWidth;
    NSInteger                           frameHeight;
    GLuint _targetTexture;
    GLuint _targetFBO;
}

- (instancetype)initWithCoder:(NSCoder *)coder {
    if (self = [super initWithCoder:coder]) {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer*) self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                        nil];
 
    }
    return self;
}

+ (Class) layerClass {
    return [CAEAGLLayer class];
}

- (void)setupContext {
   
}

- (void)setContext:(EAGLContext*)context {
    _context = context;
    [EAGLContext setCurrentContext:_context];
    [self setupContext];
    [self createBuffer];
    [self setprogram];
}

- (void)createBuffer {
    
    glGenFramebuffers(1, &_targetFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _targetFBO);
    
    glGenTextures(1, &_targetTexture);
    glBindTexture(GL_TEXTURE_2D, _targetTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 210, 210, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, _targetTexture);
    //glBindTexture(GL_TEXTURE_2D, 0); // unbind
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 210, 210, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _targetTexture, 0);
    
    
    
    glGenFramebuffers(1, &_framebuffer);
    glGenRenderbuffers(1, &_renderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer];
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_backingHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderbuffer);
        
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        NSLog(@"failed to make complete framebuffer object %x", status);
        return ;
    }
    
    GLenum glError = glGetError();
    if (GL_NO_ERROR != glError) {
        NSLog(@"failed to setup GL %x", glError);
        return;
    }
}

- (void)setprogram {
    
    if([self buildProgram:vertexShaderString fragmentShader:rgbFragmentShaderString]) {
    }
}

- (GLuint)tempTexture {
    return _targetTexture;
}


static inline GLuint compileShader(GLenum type, NSString *shaderString) {
    GLint status;
    const GLchar *sources = (GLchar *)shaderString.UTF8String;
    
    GLuint shader = glCreateShader(type);
    if (shader == 0 || shader == GL_INVALID_ENUM) {
        NSLog(@"Failed to create shader %d", type);
        return 0;
    }
    
    glShaderSource(shader, 1, &sources, NULL);
    glCompileShader(shader);
    
#ifdef DEBUG
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(shader, logLength, &logLength, log);
        NSLog(@"Shader compile log:\n%s", log);
        free(log);
    }
#endif
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        glDeleteShader(shader);
        NSLog(@"Failed to compile shader:\n");
        return 0;
    }
    
    return shader;
}

static inline BOOL validateProgram(GLuint prog)
{
    GLint status;
    
    glValidateProgram(prog);
    
#ifdef DEBUG
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program validate log:\n%s", log);
        free(log);
    }
#endif
    
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE) {
        NSLog(@"Failed to validate program %d", prog);
        return NO;
    }
    
    return YES;
}

- (void)layoutSubviews {
    [super layoutSubviews];
    frameWidth = self.bounds.size.width;
    frameHeight = self.bounds.size.height;
}


- (void)renderTexture:(GLuint)texture with:(int)width height:(int)height {
    [EAGLContext setCurrentContext:_context];
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    
  //  CGRect rect = AVMakeRectWithAspectRatioInsideRect(CGSizeMake(width, height), (CGRect){0, 0, frameWidth, frameHeight});
    //glViewport(rect.origin.x, rect.origin.y, (GLsizei)rect.size.width, (GLsizei)rect.size.height);
//    glViewport((_backingWidth - width)/2, (_backingHeight - height) / 2, width, height);
    glViewport(0, 0, width, height);
    //[_frameCopier renderFrame:_frame->pixels];
    
    glUseProgram(_filterProgram);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    static const GLfloat imageVertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f,  1.0f,
        1.0f,  1.0f,
    };
    
    GLfloat noRotationTextureCoordinates[] = {
        1.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };
    
    
    
    
    glVertexAttribPointer(filterPositionAttribute, 2, GL_FLOAT, 0, 0, imageVertices);
    glEnableVertexAttribArray(filterPositionAttribute);
    glVertexAttribPointer(filterTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, noRotationTextureCoordinates);
    glEnableVertexAttribArray(filterTextureCoordinateAttribute);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(filterInputTextureUniform, 0);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
    [_context presentRenderbuffer:GL_RENDERBUFFER];
    
    //[self renderToTemp:texture with:width height:height];
        
}

- (void) renderToTemp:(GLuint)texure with:(int)width height:(int)height{
    [EAGLContext setCurrentContext:_context];
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glViewport((_backingWidth - width)/2, (_backingHeight - height) / 2, width, height);
    glViewport(0, 0, width, height);
    //[_frameCopier renderFrame:_frame->pixels];
    
    glUseProgram(_filterProgram);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    static const GLfloat imageVertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f,  1.0f,
        1.0f,  1.0f,
    };
    
    GLfloat noRotationTextureCoordinates[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };
    
    glVertexAttribPointer(filterPositionAttribute, 2, GL_FLOAT, 0, 0, imageVertices);
    glEnableVertexAttribArray(filterPositionAttribute);
    glVertexAttribPointer(filterTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, noRotationTextureCoordinates);
    glEnableVertexAttribArray(filterTextureCoordinateAttribute);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texure);
    glUniform1i(filterInputTextureUniform, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    
    
    glBindFramebuffer(GL_FRAMEBUFFER, _targetFBO);
    glViewport(0, 0, width, height);
    // 渲染 FBO 2
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
    
//    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
//    [_context presentRenderbuffer:GL_RENDERBUFFER];
}


- (BOOL) buildProgram:(NSString*) vertexShader fragmentShader:(NSString*) fragmentShader {
    BOOL result = NO;
    GLuint vertShader = 0, fragShader = 0;
    _filterProgram = glCreateProgram();
    vertShader = compileShader(GL_VERTEX_SHADER, vertexShader);
    if (!vertShader)
        goto exit;
    fragShader = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    if (!fragShader)
        goto exit;
    
    glAttachShader(_filterProgram, vertShader);
    glAttachShader(_filterProgram, fragShader);
    
    glLinkProgram(_filterProgram);
    
    filterPositionAttribute = glGetAttribLocation(_filterProgram, "position");
    filterTextureCoordinateAttribute = glGetAttribLocation(_filterProgram, "texcoord");
    filterInputTextureUniform = glGetUniformLocation(_filterProgram, "inputImageTexture");
    
    GLint status;
    glGetProgramiv(_filterProgram, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        NSLog(@"Failed to link program %d", _filterProgram);
        goto exit;
    }
    result = validateProgram(_filterProgram);
exit:
    if (vertShader)
        glDeleteShader(vertShader);
    if (fragShader)
        glDeleteShader(fragShader);
    
    if (result) {
        NSLog(@"OK setup GL programm");
    } else {
        glDeleteProgram(_filterProgram);
        _filterProgram = 0;
    }
    return result;
}


/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
