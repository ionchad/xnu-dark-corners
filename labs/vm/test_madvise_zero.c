#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

const char *test_name_madvise_zero(void) {
    return "madvise_zero";
}

int test_run_madvise_zero(void) {
    size_t page_size = (size_t)getpagesize();
    size_t len = page_size * 4;

    void *addr = mmap(NULL, len, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANON, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    memset(addr, 0xAA, len);

    int ret = madvise(addr, len, MADV_ZERO);
    if (ret != 0) {
        int e = errno;
        printf("[madvise_zero] madvise(MADV_ZERO) failed errno=%d\n", e);
        munmap(addr, len);
        return -1;
    }

    size_t i;
    int nonzero = 0;
    unsigned char *p = (unsigned char *)addr;
    for (i = 0; i < len; i++) {
        if (p[i] != 0) {
            nonzero++;
        }
    }

    printf("[madvise_zero] non-zero bytes after MADV_ZERO: %d\n", nonzero);

    munmap(addr, len);
    return 0;
}
