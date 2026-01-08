#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

const char *test_name_mmap_sparse_access(void) {
    return "mmap_sparse_access";
}

int test_run_mmap_sparse_access(void) {
    const size_t size = 128 * 1024 * 1024; /* 128 MB */
    const size_t stride = 1024 * 1024;     /* 1 MB */

    void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    printf("[mmap_sparse_access] Mapped %zu MB anonymous region\n", size / (1024*1024));
    printf("[mmap_sparse_access] Touching every %zu KB\n", stride / 1024);

    volatile char *p = (volatile char *)addr;
    size_t i;
    int count = 0;

    for (i = 0; i < size; i += stride) {
        p[i] = (char)(i & 0xFF);
        count++;
    }

    printf("[mmap_sparse_access] Touched %d pages\n", count);

    munmap(addr, size);
    return 0;
}
