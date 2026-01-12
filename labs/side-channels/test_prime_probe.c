#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <x86intrin.h>

const char *test_name_prime_probe(void) {
    return "prime_probe";
}

static inline uint64_t rdtsc(void) {
    return __rdtsc();
}

#define SETS 64
#define WAYS 8

int test_run_prime_probe(void) {
    size_t page_size = (size_t)getpagesize();
    char *eviction_buffer = malloc(page_size * SETS * WAYS);
    if (!eviction_buffer) {
        perror("malloc");
        return -1;
    }

    memset(eviction_buffer, 0, page_size * SETS * WAYS);

    printf("[prime_probe] Simulating Prime+Probe side-channel\n");

    
    size_t i;
    for (i = 0; i < SETS * WAYS; i++) {
        volatile char tmp = eviction_buffer[i * page_size];
        (void)tmp;
    }

    printf("[prime_probe] Cache primed with %zu accesses\n", SETS * WAYS);

    
    volatile char *victim_mem = &eviction_buffer[42 * page_size];
    *victim_mem = 1;

    
    printf("[prime_probe] Probing cache sets:\n");
    for (i = 0; i < SETS * WAYS; i++) {
        uint64_t start = rdtsc();
        volatile char tmp = eviction_buffer[i * page_size];
        (void)tmp;
        uint64_t end = rdtsc();
        uint64_t time = end - start;

        if (time > 200) {
            printf("[prime_probe]   Set %zu: %llu cycles (SLOW - evicted)\n", i, time);
        }
    }

    free(eviction_buffer);
    return 0;
}
