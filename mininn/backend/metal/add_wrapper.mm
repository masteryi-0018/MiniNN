#import <Metal/Metal.h>
#import "metal.h"

void metal_add_wrapper(const float* h_A, const float* h_B, float* h_C,
                       int numElements) {
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    id<MTLCommandQueue> commandQueue = [device newCommandQueue];

    NSError *error = nil;

    // Load shader from file
    // NSString *filePath = @"mininn/backend/metal/add.metal";
    // NSString *source = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:&error];
    // if (!source) { NSLog(@"Error loading shader: %@", error); return; }
    // id<MTLLibrary> library = [device newLibraryWithSource:source options:nil error:&error];

    // pre build
    NSString* path = @"build/mininn/backend/metal/backend.metallib";
    NSURL* url = [NSURL fileURLWithPath:path];
    id<MTLLibrary> library = [device newLibraryWithURL:url error:&error];
    if (!library) {NSLog(@"Failed to load metallib: %@", error); return; }
    // end

    id<MTLFunction> function = [library newFunctionWithName:@"metal_add"];
    id<MTLComputePipelineState> pipeline = [device newComputePipelineStateWithFunction:function error:&error];

    // Create buffers
    id<MTLBuffer> xBuffer = [device newBufferWithBytes:h_A length:sizeof(float)*numElements options:MTLResourceStorageModeShared];
    id<MTLBuffer> yBuffer = [device newBufferWithBytes:h_B length:sizeof(float)*numElements options:MTLResourceStorageModeShared];
    id<MTLBuffer> outBuffer = [device newBufferWithLength:sizeof(float)*numElements options:MTLResourceStorageModeShared];

    id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
    id<MTLComputeCommandEncoder> encoder = [commandBuffer computeCommandEncoder];
    [encoder setComputePipelineState:pipeline];
    [encoder setBuffer:xBuffer offset:0 atIndex:0];
    [encoder setBuffer:yBuffer offset:0 atIndex:1];
    [encoder setBuffer:outBuffer offset:0 atIndex:2];

    MTLSize gridSize = MTLSizeMake(numElements, 1, 1);
    NSUInteger threadGroupSize = pipeline.maxTotalThreadsPerThreadgroup;
    if (threadGroupSize > numElements) threadGroupSize = numElements;
    MTLSize threadgroupSize = MTLSizeMake(threadGroupSize, 1, 1);

    [encoder dispatchThreads:gridSize threadsPerThreadgroup:threadgroupSize];
    [encoder endEncoding];
    [commandBuffer commit];
    [commandBuffer waitUntilCompleted];

    memcpy(h_C, [outBuffer contents], sizeof(float)*numElements);
}