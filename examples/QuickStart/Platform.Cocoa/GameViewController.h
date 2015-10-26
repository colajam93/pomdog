#import <Cocoa/Cocoa.h>
#import "GameView.h"

@interface GameViewController : NSViewController<MTKViewDelegate>

@property (assign) IBOutlet GameView *gameView;

@end
