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
#include "GLJoinFilter.hpp"
#include "GLTransitionFilter.hpp"
#include "GLOverlayFilter.hpp"

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
        
        if (!_glContext) {
            EAGLContext *glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
            if (!glContext) {
              glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
            }
            _glContext = glContext;
        }

        
        [self.videoView setContext:self.glContext];
    
        
//        NSString* yuv_filePath = [[NSBundle mainBundle] pathForResource:@"trailer_i420" ofType:@"yuv"];
//        I420Reader reader(yuv_filePath.UTF8String, 854, 480);
//        GLTextureReader textReader(&reader, glContext);
//        GLFileInput glInput(textReader);
        
        GLScreen screen([self](GLTextureFrame* textFrame) {
            //[self.videoView renderTexture:textFrame->getTexture() with:textFrame->getWidth() height:textFrame->getHeight()];
            dispatch_async(dispatch_get_main_queue(), ^{
                [self.videoView renderTexture:textFrame->getTexture() with:textFrame->getWidth() height:textFrame->getHeight()];                
            });
        });
        
        NSString* filePath = [[NSBundle mainBundle] pathForResource:@"8288_short" ofType:@"mp4"];
        AssetReader reader1(filePath.UTF8String, self.glContext);
        GLFileInput glInput1(reader1);
        GLGrayFilter grayfilter;
        
        NSString* filePath2 = [[NSBundle mainBundle] pathForResource:@"45678" ofType:@"mp4"];
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
    });
    
}
@end
