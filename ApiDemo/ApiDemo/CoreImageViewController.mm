//
//  CoreImageViewController.m
//  ApiDemo
//
//  Created by Bruce on 2023/5/18.
//  Copyright © 2023 Grocery. All rights reserved.
//

#import "CoreImageViewController.h"
#import "VideoView.h"
#import "NV12Reader.hpp"
#import "CIImageReader.hpp"
#import "CIFileInput.hpp"
#import "CIFilterScale.hpp"
#import "CIFilterOverlay.hpp"
#import "CIFilterGray.hpp"
#import "CIScreen.hpp"

@interface CoreImageViewController ()
@property (weak, nonatomic) IBOutlet VideoView *videoView;
@property (nonatomic, strong) dispatch_queue_t dispatch_queue;
@end

@implementation CoreImageViewController

- (void)viewDidLoad {
    [super viewDidLoad];
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
            NSString* yuvPath = [[NSBundle mainBundle] pathForResource:@"trailer_nv12" ofType:@"yuv"];
            NV12Reader nv12_reader0(yuvPath.UTF8String, 854, 480);
            NV12Reader nv12_reader1(yuvPath.UTF8String, 854, 480);
            CIImageReader reader0(&nv12_reader0);
            CIImageReader reader1(&nv12_reader1);
    
            CIFileInput input0(reader0);
            CIFileInput input1(reader0);
            CIFilterScale scale;
            CIFilterOverlay overly;
            CIFilterGray gary;
    
            CIScreen screen([self](CIImage* frame){
                [self.videoView renderFame:frame];
            });
    
    
            input0.connect(overly, 0);
            input1.connect(scale, 0);
            scale.connect(gary, 0);
            gary.connect(overly, 1);
            overly.connect(screen, 0);
            screen.start();
        });
    
}

@end
