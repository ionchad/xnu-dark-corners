#include <stdio.h>
#include <stdlib.h>


const char *test_name_msync_weird(void);
int test_run_msync_weird(void);

const char *test_name_madvise_zero(void);
int test_run_madvise_zero(void);

const char *test_name_madvise_dontneed(void);
int test_run_madvise_dontneed(void);

typedef struct {
    const char *(*name_fn)(void);
    int (*run_fn)(void);
} vm_test_case;

static vm_test_case vm_tests[] = {
    { test_name_msync_weird,        test_run_msync_weird        },
    { test_name_madvise_zero,       test_run_madvise_zero       },
    { test_name_madvise_dontneed,   test_run_madvise_dontneed   },
};

static const size_t vm_test_count = sizeof(vm_tests) / sizeof(vm_tests[0]);

int vm_run_all(void) {
    printf("[VM] running %zu tests\n", vm_test_count);
    size_t i;
    int failures = 0;

    for (i = 0; i < vm_test_count; i++) {
        const char *name = vm_tests[i].name_fn();
        printf("=== [VM %zu/%zu] %s ===\n", i + 1, vm_test_count, name);
        int ret = vm_tests[i].run_fn();
        if (ret == 0) {
            printf("[VM PASS] %s\n\n", name);
        } else {
            printf("[VM FAIL] %s (code=%d)\n\n", name, ret);
            failures++;
        }
    }

    printf("[VM] Done. Failures: %d\n", failures);
    return failures == 0 ? 0 : 1;
}
