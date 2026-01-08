#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

const char *test_name_mmap_large_file(void) {
    return "mmap_large_file";
}

int test_run_mmap_large_file(void) {
    const char *path = "large_test_file.bin";
    const size_t size = 64 * 1024 * 1024; /* 64 MB */

    int fd = open(path, O_RDWR | O_CREAT, 0600);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    if (ftruncate(fd, size) != 0) {
        perror("ftruncate");
        close(fd);
        return -1;
    }

    void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        unlink(path);
        return -1;
    }

    printf("[mmap_large_file] Mapped %zu MB file at %p\n", size / (1024*1024), addr);

    volatile char *p = (volatile char *)addr;
    p[0] = 'A';
    p[size - 1] = 'Z';

    printf("[mmap_large_file] Touched first and last page\n");

    munmap(addr, size);
    close(fd);
    unlink(path);
    return 0;
}
