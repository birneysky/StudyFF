//
//  VideoView.h
//  AVEditorApp
//
//  Created by Bruce on 2023/5/11.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface VideoView : UIView

- (void)renderFame:(CIImage*)frame;

@end

NS_ASSUME_NONNULL_END
