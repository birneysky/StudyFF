//
//  VideoView.m
//  AVEditorApp
//
//  Created by Bruce on 2023/5/11.
//

#import "VideoView.h"
#import <AVFoundation/AVFoundation.h>

@interface VideoView ()
@property (nonatomic, strong) AVSampleBufferDisplayLayer* dispalyLayer;
@end

@implementation VideoView {
    CIContext *_context;
}
//
//+(Class)layerClass {
//    return [AVSampleBufferDisplayLayer class];
//}

- (instancetype)initWithCoder:(NSCoder *)coder {
    if (self = [super initWithCoder:coder]) {
        [self setup];
    }
    return self;
}

- (instancetype)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        [self setup];
    }
    return self;
}


- (void)setup {
    self.backgroundColor = [UIColor systemPinkColor];
    _dispalyLayer = [[AVSampleBufferDisplayLayer alloc] init];
    _dispalyLayer.frame = self.bounds;
    _dispalyLayer.backgroundColor = [UIColor blueColor].CGColor;
    _dispalyLayer.position = CGPointMake(CGRectGetMidX(self.bounds), CGRectGetMidY(self.bounds));
//    [self.dispalyLayer flushAndRemoveImage];
    self.dispalyLayer.videoGravity = AVLayerVideoGravityResizeAspect;
    [self.layer addSublayer:self.dispalyLayer];
    //self.layer.allowContentsRectChange = YES;
//    self.layer.masksToBounds = NO;
    self.layer.opaque = YES;
    _context = [CIContext context];
}

- (void)renderFame:(CIImage*)frame{
    CIImage* image = frame;

    
    NSDictionary *pixelAttributes = @{(id)kCVPixelBufferIOSurfacePropertiesKey : @{},
        //(id)kCVPixelBufferBytesPerRowAlignmentKey:@(16),
        (id)kCVPixelBufferWidthKey:@(image.extent.size.width),
        (id)kCVPixelBufferHeightKey:@(image.extent.size.height)
    };
    
    CVPixelBufferRef pixelBuffer;
    CVPixelBufferCreate(kCFAllocatorDefault, (int)image.extent.size.width, (int)image.extent.size.height,
                        kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange, (__bridge CFDictionaryRef _Nullable)(pixelAttributes), &pixelBuffer);
    [_context render:image toCVPixelBuffer:pixelBuffer];

    CMSampleTimingInfo timing = {kCMTimeInvalid, kCMTimeInvalid, kCMTimeInvalid};

    CMVideoFormatDescriptionRef videoInfo = nil;
    OSStatus result = CMVideoFormatDescriptionCreateForImageBuffer(nil, pixelBuffer, &videoInfo);
    NSParameterAssert(result == 0 && videoInfo != nil);

    CMSampleBufferRef sampleBuffer = nil;
    result = CMSampleBufferCreateForImageBuffer(kCFAllocatorDefault, pixelBuffer, true, NULL, NULL, videoInfo, &timing, &sampleBuffer);
    NSParameterAssert(result == 0 && sampleBuffer != nil);
    CFRelease(videoInfo);

    CFArrayRef attachments = CMSampleBufferGetSampleAttachmentsArray(sampleBuffer, YES);
    CFMutableDictionaryRef dict = (CFMutableDictionaryRef) CFArrayGetValueAtIndex(attachments, 0);
    CFDictionarySetValue(dict, kCMSampleAttachmentKey_DisplayImmediately, kCFBooleanTrue);
    
    dispatch_async(dispatch_get_main_queue(), ^{
        AVQueuedSampleBufferRenderingStatus status = self.dispalyLayer.status;
        if (status != AVQueuedSampleBufferRenderingStatusRendering) {
            [self.dispalyLayer flush];
        }
        if ([self.dispalyLayer isReadyForMoreMediaData]) {
            [self.dispalyLayer enqueueSampleBuffer:sampleBuffer];
        } else {
            
        }
        
        CVPixelBufferRelease(pixelBuffer);
        if (sampleBuffer) {
            CFRelease(sampleBuffer);
        }
    });
    
    
}


- (AVSampleBufferDisplayLayer*)dispalyLayer {
    return (AVSampleBufferDisplayLayer*)_dispalyLayer;
}

//- (CGSize)intrinsicContentSize {
//    return CGSizeMake(UIViewNoIntrinsicMetric, UIViewNoIntrinsicMetric);
//}

- (void)dealloc {
    NSLog(@"######### dealloc %p", self);
}


- (void)layoutSubviews {
    [super layoutSubviews];
    self.dispalyLayer.frame = self.bounds;
}

//- (void)layoutSublayersOfLayer:(CALayer *)layer {
//    self.dispalyLayer.frame = self.bounds;
//}

@end
