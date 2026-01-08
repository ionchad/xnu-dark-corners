#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

const char *test_name_mmap_hint_addresses(void) {
    return "mmap_hint_addresses";
}

int test_run_mmap_hint_addresses(void) {
    const size_t size = 4096;
    void *hints[] = {
        NULL,
        (void *)0x100000000UL,
        (void *)0x200000000UL,
        (void *)0x7fff00000000UL,
    };
    size_t i;

    printf("[mmap_hint_addresses] Testing address hints\n");

    for (i = 0; i < sizeof(hints) / sizeof(hints[0]); i++) {
        void *hint = hints[i];
        void *addr = mmap(hint, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);

        if (addr == MAP_FAILED) {
            printf("[mmap_hint_addresses] hint=%p: mmap failed errno=%d\n", hint, errno);
        } else {
            printf("[mmap_hint_addresses] hint=%p: got %p (delta=%ld)\n",
                   hint, addr, (long)((char *)addr - (char *)hint));
            munmap(addr, size);
        }
    }

    return 0;
}
