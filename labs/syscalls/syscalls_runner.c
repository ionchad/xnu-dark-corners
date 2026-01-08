#include <stdio.h>
#include <stdlib.h>

const char *test_name_unknown_syscall(void);
int test_run_unknown_syscall(void);

const char *test_name_syscall_boundary(void);
int test_run_syscall_boundary(void);

typedef struct {
    const char *(*name_fn)(void);
    int (*run_fn)(void);
} syscalls_test_case;

static syscalls_test_case syscalls_tests[] = {
    { test_name_unknown_syscall,   test_run_unknown_syscall   },
    { test_name_syscall_boundary,  test_run_syscall_boundary  },
};

static const size_t syscalls_test_count = sizeof(syscalls_tests) / sizeof(syscalls_tests[0]);

int syscalls_run_all(void) {
    printf("[SYSCALLS] running %zu tests\n", syscalls_test_count);
    size_t i;
    int failures = 0;

    for (i = 0; i < syscalls_test_count; i++) {
        const char *name = syscalls_tests[i].name_fn();
        printf("[SYSCALLS %zu/%zu] %s \n", i + 1, syscalls_test_count, name);
        int ret = syscalls_tests[i].run_fn();
        if (ret == 0) {
            printf("[SYSCALLS PASS] %s\n\n", name);
        } else {
            printf("[SYSCALLS FAIL] %s (code=%d)\n\n", name, ret);
            failures++;
        }
    }

    printf("[SYSCALLS] Done. Failures: %d\n", failures);
    return failures == 0 ? 0 : 1;
}
