//
//  TextureRender.h
//  OpenGLRenderTexture
//
//  Created by Bruce on 2023/5/16.
//

#import <UIKit/UIKit.h>


NS_ASSUME_NONNULL_BEGIN

@interface TextureRender : UIView

- (void)setContext:(EAGLContext*)context;

- (void)renderTexture:(GLuint)texture with:(int)width height:(int)height;

- (GLuint)tempTexture;

@end

NS_ASSUME_NONNULL_END
