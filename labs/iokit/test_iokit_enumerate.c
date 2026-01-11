#include <stdio.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>

const char *test_name_iokit_enumerate(void) {
    return "iokit_enumerate";
}

int test_run_iokit_enumerate(void) {
    io_iterator_t iter;
    kern_return_t kr;

    kr = IOServiceGetMatchingServices(kIOMasterPortDefault,
                                      IOServiceMatching("IOPlatformExpertDevice"),
                                      &iter);
    if (kr != KERN_SUCCESS) {
        printf("[iokit_enumerate] IOServiceGetMatchingServices failed: 0x%x\n", kr);
        return -1;
    }

    io_service_t service;
    int count = 0;

    while ((service = IOIteratorNext(iter)) != 0) {
        count++;
        
        io_name_t name;
        kr = IORegistryEntryGetName(service, name);
        if (kr == KERN_SUCCESS) {
            printf("[iokit_enumerate] Found service: %s\n", name);
        } else {
            printf("[iokit_enumerate] Found service (name read failed)\n");
        }
        
        IOObjectRelease(service);
    }

    IOObjectRelease(iter);
    printf("[iokit_enumerate] Enumerated %d IOPlatformExpertDevice services\n", count);
    return 0;
}
