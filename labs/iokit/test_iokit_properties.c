#include <stdio.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>

const char *test_name_iokit_properties(void) {
    return "iokit_properties";
}

static void print_cf_value(CFTypeRef val) {
    if (CFGetTypeID(val) == CFStringGetTypeID()) {
        char buf[256];
        if (CFStringGetCString((CFStringRef)val, buf, sizeof(buf), kCFStringEncodingUTF8)) {
            printf("%s", buf);
        }
    } else if (CFGetTypeID(val) == CFNumberGetTypeID()) {
        long long num;
        CFNumberGetValue((CFNumberRef)val, kCFNumberLongLongType, &num);
        printf("%lld", num);
    } else if (CFGetTypeID(val) == CFDataGetTypeID()) {
        printf("<data %ld bytes>", CFDataGetLength((CFDataRef)val));
    } else {
        printf("<other type>");
    }
}

int test_run_iokit_properties(void) {
    io_service_t service = IOServiceGetMatchingService(kIOMasterPortDefault,
                                                       IOServiceMatching("IOPlatformExpertDevice"));
    if (!service) {
        printf("[iokit_properties] Could not find IOPlatformExpertDevice\n");
        return -1;
    }

    const char *props[] = { "model", "manufacturer", "IOPlatformSerialNumber" };
    size_t i;

    for (i = 0; i < sizeof(props) / sizeof(props[0]); i++) {
        CFStringRef key = CFStringCreateWithCString(NULL, props[i], kCFStringEncodingUTF8);
        CFTypeRef val = IORegistryEntryCreateCFProperty(service, key, kCFAllocatorDefault, 0);
        
        printf("[iokit_properties] %s = ", props[i]);
        if (val) {
            print_cf_value(val);
            printf("\n");
            CFRelease(val);
        } else {
            printf("<not found>\n");
        }
        
        CFRelease(key);
    }

    IOObjectRelease(service);
    return 0;
}
