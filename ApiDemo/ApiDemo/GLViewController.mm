//
//  GLViewController.m
//  ApiDemo
//
//  Created by Bruce on 2023/5/18.
//  Copyright © 2023 Grocery. All rights reserved.
//

#import "GLViewController.h"
#import "TextureRenderView.h"
#include "AssetReader.hpp"
#include "GLFileInput.hpp"
#include "GLScreen.hpp"
#include "I420Reader.hpp"
#include "GLGrayFilter.hpp"

@interface GLViewController ()
@property (weak, nonatomic) IBOutlet TextureRenderView *videoView;
@property (nonatomic, strong) dispatch_queue_t dispatch_queue;
@property (nonatomic, strong) EAGLContext* glContext;
@end

@implementation GLViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    
//    reader.startReading();
//    reader.getNextFrame(AssetReader::MediaType::Video);
    // Do any additional setup after loading the view.
}

- (dispatch_queue_t)dispatch_queue {
    if (!_dispatch_queue) {
        _dispatch_queue = dispatch_queue_create("com.render.coreimage", DISPATCH_QUEUE_SERIAL);
    }
    return _dispatch_queue;
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (IBAction)startAction:(id)sender {
    dispatch_async(self.dispatch_queue, ^{
        
        EAGLContext *glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        if (!glContext) {
          glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        }
        
        self.glContext = glContext;
        
        [self.videoView setContext:self.glContext];
    
        
//        NSString* yuv_filePath = [[NSBundle mainBundle] pathForResource:@"trailer_i420" ofType:@"yuv"];
//        I420Reader reader(yuv_filePath.UTF8String, 854, 480);
//        GLTextureReader textReader(&reader, glContext);
//        GLFileInput glInput(textReader);
        NSString* filePath = [[NSBundle mainBundle] pathForResource:@"123456" ofType:@"mp4"];
        AssetReader reader(filePath.UTF8String, self.glContext);
        GLFileInput glInput(reader);
        GLGrayFilter grayfilter;
        GLScreen screen([self](GLTextureFrame* textFrame) {
            //[self.videoView renderTexture:textFrame->getTexture() with:textFrame->getWidth() height:textFrame->getHeight()];
            [self.videoView renderTexture:textFrame->getTexture() with:textFrame->getWidth() height:textFrame->getHeight()];
        });
        glInput.connect(grayfilter, 0);
        grayfilter.connect(screen, 0);
        screen.start();
        NSLog(@"i'm done");
    });
    
}
@end
