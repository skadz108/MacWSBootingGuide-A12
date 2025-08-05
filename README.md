# MacWSBootingGuide
Booting macOS's WindowServer on your jailbroken iDevice for real (WIP)

You need these from simulator runtime: MTLSimDriver.framework, MTLSimImplementation.framework, MetalSerializer.framework

## Setting up
- TODO

## Additional patches
> [!NOTE]
> - [x] means it is automated or handled by hooks
> - [ ] means you need to patch it by hand

### macOS side
#### dyld
- [ ] `mach-o file, but is an incompatible architecture (have 'arm64e', need 'arm64')` because `GradedArchs::grade` [disallows](https://github.com/apple-oss-distributions/dyld/blob/dyld-1285.19/common/MachOFile.cpp#L1985-L1989) loading non-system arm64e libraries to arm64 processes. (not really this function but the caller of it I forgot).

#### launchservicesd
- [x] Missing syscalls: `audit_token_to_asid`, `audit_token_to_auid`, `auditon`, `getaudit_addr`
- [ ] This daemon needs to be converted to a dylib using [LiveContainer's method](https://github.com/LiveContainer/LiveContainer/blob/341cc87d40d8eec690d21dc71bd69d74667588da/LiveContainer/LCMachOUtils.m#L71-L88)

#### MTLSimDriver
- [x] `failed assertion _limits.maxColorAttachments > 0 at line 3791 in -[_MTLDevice initLimits]`, can be bypassed using `CFPreferencesSetAppValue(@"EnableSimApple5", @1, @"com.apple.Metal")`
- [x] `-[MTLTextureDescriptorInternal validateWithDevice:], line 1344: error 'Texture Descriptor Validation invalid storageMode (1). Must be one of MTLStorageModeShared(0) MTLStorageModeMemoryless(3) MTLStorageModePrivate(2)`: because macOS defaults to `MTLStorageModeManaged`, while iOS always has unified memory so it doesn't allow that.
- [x] `Attempt to pass a malloc(3)ed region to xpc_shmem_create().`: while regular drivers accept passing `malloc`ed region to `newBufferWithBytesNoCopy:length:options:deallocator:`, doing so to simulator is not allowed since XPC has to share the memory with `MTLSimDriverHost.xpc` process. Workaround is to create a mirrored region using `vm_remap` that can be shared across processes.
- [x] `Unimplemented pixel format of 645346401 used in WSCompositeDestinationCreateWithIOSurface.` due to missing implementation of `-[MTLSimDevice acceleratorPort]`, which mysteriously caused WindowServer to fallback to software rendering in some places, causing said fatal error.
- [ ] `-[MTLSimTexture initWithDescriptor:decompressedPixelFormat:iosurface:plane:textureRef:heap:device:]:813: failed assertion 'IOSurface backed XR10 textures are not supported in the simulator'`: patch out the check, since it actually works fine.
- [ ] `-[MTLSimBuffer newTextureWithDescriptor:offset:bytesPerRow:]`: patch `storageMode == private` check.
- [ ] `-[MTLSimDevice newRenderPipelineStateWithTileDescriptor:options:reflection:error:], line 2124: error 'not supported in the simulator'`. FIXME: this is not implemented at all. However, it is only used by `QuartzCore'CA::OGL::BlurState::tile_downsample(int)` which can be skipped.

#### WindowServer
- [x] It hangs twice when calling `NXClickTime` and `NXGetClickSpace`. Hooked to do nothing instead since both were deprecated.

### iOS side
#### MTLCompilerService
- [x] `MTLCompilerObject::readModuleFromBinaryRequest`: patch platform check to allow cross-platform compilation. MTLCompilerBypassOSCheck compares against hardcoded instruction so it might not be reliable across iOS versions.

#### launchd
- [ ] `Path not allowed in target domain` is raised when attempting to load XPC bundles not declared in `launchd.plist` (`MTLSimDriverHost.xpc` in this case).

#### watchdogd
- [x] Install `WatchDisable` tweak from [this repo](https://nathan4s.lol/repo) which automatically runs @zhuowei's `who_let_the_dogs_out.c` at boot.

## Credits
- [zhuowei/iOS-run-macOS-executables-tools](https://github.com/zhuowei/iOS-run-macOS-executables-tools)
- [SongXiaoXi/Reductant](https://github.com/SongXiaoXi/Reductant)
