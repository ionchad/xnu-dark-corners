#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <x86intrin.h>

const char *test_name_cache_timing(void) {
    return "cache_timing";
}

#define SAMPLES 1000

static inline uint64_t rdtsc(void) {
    return __rdtsc();
}

int test_run_cache_timing(void) {
    size_t page_size = (size_t)getpagesize();
    void *mem = malloc(page_size * 4);
    if (!mem) {
        perror("malloc");
        return -1;
    }

    memset(mem, 0, page_size * 4);

    printf("[cache_timing] Measuring cache vs memory access timing\n");

    volatile char *p = (volatile char *)mem;
    uint64_t cached_times[SAMPLES];
    uint64_t uncached_times[SAMPLES];
    size_t i;

    
    for (i = 0; i < SAMPLES; i++) {
        p[0] = 1; 
        uint64_t start = rdtsc();
        (void)p[0];
        uint64_t end = rdtsc();
        cached_times[i] = end - start;
    }

    
    for (i = 0; i < SAMPLES; i++) {
        _mm_clflush((void *)&p[0]); 
        _mm_mfence();
        uint64_t start = rdtsc();
        (void)p[0];
        uint64_t end = rdtsc();
        uncached_times[i] = end - start;
    }

    
    uint64_t cached_sum = 0, uncached_sum = 0;
    for (i = 0; i < SAMPLES; i++) {
        cached_sum += cached_times[i];
        uncached_sum += uncached_times[i];
    }

    printf("[cache_timing] Cached access avg:   %llu cycles\n", cached_sum / SAMPLES);
    printf("[cache_timing] Uncached access avg: %llu cycles\n", uncached_sum / SAMPLES);

    free(mem);
    return 0;
}
