#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

const char *test_name_msync_weird(void) {
    return "msync_weird";
}

static int do_msync(void *addr, size_t len, int flags, const char *label) {
    int ret = msync(addr, len, flags);
    if (ret < 0) {
        int e = errno;
        printf("[msync_weird] %s: msync failed errno=%d\n", label, e);
        return -1;
    }
    printf("[msync_weird] %s: msync succeeded\n", label);
    return 0;
}

int test_run_msync_weird(void) {
    size_t page_size = (size_t)getpagesize();

    void *addr = mmap(NULL, page_size, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANON, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    strcpy((char *)addr, "hello from msync_weird");

    int failures = 0;

    if (do_msync(addr, page_size, MS_SYNC, "anon_full_page_MS_SYNC") != 0)
        failures++;

    if (do_msync(addr, page_size / 2, MS_ASYNC, "anon_half_page_MS_ASYNC") != 0)
        failures++;

    if (do_msync((char *)addr + (page_size / 2), page_size / 2,
                 MS_INVALIDATE, "anon_half_page_MS_INVALIDATE") != 0)
        failures++;

    if (munmap(addr, page_size) < 0) {
        perror("munmap");
        return -1;
    }

    return failures == 0 ? 0 : 1;
}
