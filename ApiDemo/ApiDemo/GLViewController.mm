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
#include "GLPlayer.hpp"
#include "I420Reader.hpp"
#include "GLGrayFilter.hpp"
#include "GLJoinFilter.hpp"
#include "GLTransitionFilter.hpp"
#include "GLOverlayFilter.hpp"

@interface GLViewController ()
@property (weak, nonatomic) IBOutlet TextureRenderView *videoView;
@property (nonatomic, strong) dispatch_queue_t dispatch_queue;
@property (nonatomic, strong) EAGLContext* glContext;
@property (nonatomic,  assign) GLPlayer* screen;
@end

@implementation GLViewController {
    
}

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
    
    __weak typeof(self) weakSelf = self;
    
    dispatch_async(self.dispatch_queue, ^{
        
        if (!weakSelf.glContext) {
            EAGLContext *glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
            if (!glContext) {
              glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
            }
            _glContext = glContext;
        }

        
        [weakSelf.videoView setContext:weakSelf.glContext];
    
        
//        NSString* yuv_filePath = [[NSBundle mainBundle] pathForResource:@"trailer_i420" ofType:@"yuv"];
//        I420Reader reader(yuv_filePath.UTF8String, 854, 480);
//        GLTextureReader textReader(&reader, glContext);
//        GLFileInput glInput(textReader);
        
        GLPlayer screen([weakSelf](GLTextureFrame* textFrame) {
            //[self.videoView renderTexture:textFrame->getTexture() with:textFrame->getWidth() height:textFrame->getHeight()];
            //dispatch_async(dispatch_get_main_queue(), ^{
                [weakSelf.videoView renderTexture:textFrame->getTexture() with:textFrame->getWidth() height:textFrame->getHeight()];
            ///});
        });
        
        
        weakSelf.screen = &screen;
        NSString* filePath = [[NSBundle mainBundle] pathForResource:@"04_quasar" ofType:@"mp4"];
        AssetReader reader1(filePath.UTF8String, self.glContext);
        GLFileInput glInput1(reader1);
        GLGrayFilter grayfilter;
        
        NSString* filePath2 = [[NSBundle mainBundle] pathForResource:@"02_blackhole" ofType:@"mp4"];
        AssetReader reader2(filePath2.UTF8String, self.glContext);
        GLFileInput glInput2(reader2);
        
        
        NSString* filePath3 = [[NSBundle mainBundle] pathForResource:@"trailer" ofType:@"mp4"];
        AssetReader reader3(filePath3.UTF8String, self.glContext);
        GLFileInput glInput3(reader3);
        
        //GLJoinFilter joinFilter;
        GLTransitionFilter transitionFilter;
        GLOverlayFilter overlayFilter;
        
        glInput1.connect(grayfilter, 0);
    
        grayfilter.connect(transitionFilter, 0);
        glInput2.connect(transitionFilter, 1);
        
        transitionFilter.connect(overlayFilter,0);
        glInput3.connect(overlayFilter, 1);
        overlayFilter.connect(screen, 0);
        
        screen.start();
        NSLog(@"i'm done");
        weakSelf.screen = nullptr;
    });
}
- (IBAction)pauseAction:(id)sender {
    if (self.screen) {
        self.screen->pause();
    }
}

- (IBAction)resumeAction:(id)sender {
    if (self.screen) {
        self.screen->resume();
    }
}
@end
