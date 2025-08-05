ARCHS = arm64
include $(THEOS)/makefiles/common.mk

BUNDLE_NAME = MTLSimDriverHost

MTLSimDriverHost_DYNAMIC_LIBRARY = 0
MTLSimDriverHost_BUNDLE_EXTENSION = xpc
MTLSimDriverHost_FILES = main.x
MTLSimDriverHost_CFLAGS = -fobjc-arc
MTLSimDriverHost_CODESIGN_FLAGS = -S../entitlements.plist
MTLSimDriverHost_INSTALL_PATH = /usr/macOS/Frameworks/MTLSimDriver.framework/XPCServices

include $(THEOS_MAKE_PATH)/bundle.mk
