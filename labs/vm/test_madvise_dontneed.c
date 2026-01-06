#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

const char *test_name_madvise_dontneed(void) {
    return "madvise_dontneed";
}

static double now_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

int test_run_madvise_dontneed(void) {
    const char *path = "vm_test_file.bin";
    const size_t len = 4 * 1024 * 1024; 

    int fd = open(path, O_RDWR | O_CREAT, 0600);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    if (ftruncate(fd, len) != 0) {
        perror("ftruncate");
        close(fd);
        return -1;
    }

    void *addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return -1;
    }

    memset(addr, 0xAB, len);

    volatile unsigned char *p = (volatile unsigned char *)addr;
    size_t i;

    double t1 = now_sec();
    for (i = 0; i < len; i += 4096) {
        (void)p[i];
    }
    double t2 = now_sec();
    double first_scan = t2 - t1;
    printf("[madvise_dontneed] first scan time: %.6f sec\n", first_scan);

    if (madvise(addr, len, MADV_DONTNEED) != 0) {
        perror("madvise(MADV_DONTNEED)");
    }

    double t3 = now_sec();
    for (i = 0; i < len; i += 4096) {
        (void)p[i];
    }
    double t4 = now_sec();
    double second_scan = t4 - t3;
    printf("[madvise_dontneed] second scan time after MADV_DONTNEED: %.6f sec\n",
           second_scan);

    munmap(addr, len);
    close(fd);
    unlink(path);

    return 0;
}
