#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <x86intrin.h>

const char *test_name_flush_reload(void) {
    return "flush_reload";
}

static inline uint64_t rdtsc(void) {
    return __rdtsc();
}

#define THRESHOLD 100

int test_run_flush_reload(void) {
    size_t page_size = (size_t)getpagesize();
    char *probe_array = malloc(page_size * 256);
    if (!probe_array) {
        perror("malloc");
        return -1;
    }

    memset(probe_array, 0, page_size * 256);

    printf("[flush_reload] Simulating Flush+Reload side-channel\n");

    
    size_t secret = 42;
    volatile char *victim_access = &probe_array[secret * page_size];
    *victim_access = 1; 

    
    size_t i;
    for (i = 0; i < 256; i++) {
        _mm_clflush((void *)&probe_array[i * page_size]);
    }
    _mm_mfence();

    
    *victim_access = 2;

    
    printf("[flush_reload] Probing which cache line was accessed:\n");
    for (i = 0; i < 256; i++) {
        uint64_t start = rdtsc();
        volatile char tmp = probe_array[i * page_size];
        (void)tmp;
        uint64_t end = rdtsc();
        uint64_t time = end - start;

        if (time < THRESHOLD) {
            printf("[flush_reload]   Index %zu: %llu cycles (FAST - likely accessed)\n", i, time);
        }
    }

    free(probe_array);
    return 0;
}
