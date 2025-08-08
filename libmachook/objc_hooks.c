// Can't include any other headers since it will trip "unavailable on iOS" error
#import "interpose.h"

typedef unsigned long long uintptr_t;
uintptr_t objc_addExceptionHandler(void *fn, void *context);
void objc_removeExceptionHandler(uintptr_t token);

// workaround strange SIGTRAPs
uintptr_t objc_addExceptionHandler_new(void *fn, void *context) {
    return 0;
}
void objc_removeExceptionHandler_new(uintptr_t token) {
    // do nothing
}

DYLD_INTERPOSE(objc_addExceptionHandler_new, objc_addExceptionHandler);
DYLD_INTERPOSE(objc_removeExceptionHandler_new, objc_removeExceptionHandler);
