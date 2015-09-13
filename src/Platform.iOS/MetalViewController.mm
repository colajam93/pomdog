// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#import "MetalViewController.hpp"

@implementation MetalViewController
{
    // view
    MTKView* _view;

    // controller
    dispatch_semaphore_t _inflight_semaphore;
    id <MTLBuffer> _dynamicConstantBuffer;
    uint8_t _constantDataBufferIndex;

    // renderer
    id <MTLDevice> _device;
    id <MTLCommandQueue> _commandQueue;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    _constantDataBufferIndex = 0;
    _inflight_semaphore = dispatch_semaphore_create(3);

    [self _setupMetal];
    [self _setupView];
    [self _loadAssets];
    [self _reshape];
}

- (void)_setupView
{
    _view = (MTKView*)self.view;
    _view.device = _device;
    _view.delegate = self;

    _view.colorPixelFormat = MTLPixelFormatRGBA8Unorm;
    _view.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
}

- (void)_setupMetal
{
    _device = MTLCreateSystemDefaultDevice();
    _commandQueue = [_device newCommandQueue];
}

- (void)_loadAssets
{
}

- (void)_render
{
    dispatch_semaphore_wait(_inflight_semaphore, DISPATCH_TIME_FOREVER);

    [self _update];

    id <MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";

    MTLRenderPassDescriptor* renderPassDescriptor = _view.currentRenderPassDescriptor;

    if (renderPassDescriptor == nil) {
        return;
    }

    id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    renderEncoder.label = @"MyRenderEncoder";
    [renderEncoder pushDebugGroup:@"DrawCube"];
    [renderEncoder popDebugGroup];

    [renderEncoder endEncoding];

    __block dispatch_semaphore_t block_sema = _inflight_semaphore;
    [commandBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
        dispatch_semaphore_signal(block_sema);
    }];

    [commandBuffer presentDrawable:_view.currentDrawable];

    [commandBuffer commit];
}

- (void)_reshape
{
}

- (void)_update
{
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    [self _reshape];
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
    @autoreleasepool {
        [self _render];
    }
}

@end
