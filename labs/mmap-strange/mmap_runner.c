#include <stdio.h>
#include <stdlib.h>

const char *test_name_mmap_large_file(void);
int test_run_mmap_large_file(void);

const char *test_name_mmap_sparse_access(void);
int test_run_mmap_sparse_access(void);

const char *test_name_mmap_hint_addresses(void);
int test_run_mmap_hint_addresses(void);

typedef struct {
    const char *(*name_fn)(void);
    int (*run_fn)(void);
} mmap_test_case;

static mmap_test_case mmap_tests[] = {
    { test_name_mmap_large_file,     test_run_mmap_large_file     },
    { test_name_mmap_sparse_access,  test_run_mmap_sparse_access  },
    { test_name_mmap_hint_addresses, test_run_mmap_hint_addresses },
};

static const size_t mmap_test_count = sizeof(mmap_tests) / sizeof(mmap_tests[0]);

int mmap_run_all(void) {
    printf("[MMAP] running %zu tests\n", mmap_test_count);
    size_t i;
    int failures = 0;

    for (i = 0; i < mmap_test_count; i++) {
        const char *name = mmap_tests[i].name_fn();
        printf("[MMAP %zu/%zu] %s \n", i + 1, mmap_test_count, name);
        int ret = mmap_tests[i].run_fn();
        if (ret == 0) {
            printf("[MMAP PASS] %s\n\n", name);
        } else {
            printf("[MMAP FAIL] %s (code=%d)\n\n", name, ret);
            failures++;
        }
    }

    printf("[MMAP] Done. Failures: %d\n", failures);
    return failures == 0 ? 0 : 1;
}
