#include <stdio.h>
#include <stdlib.h>

const char *test_name_iokit_enumerate(void);
int test_run_iokit_enumerate(void);

const char *test_name_iokit_properties(void);
int test_run_iokit_properties(void);

typedef struct {
    const char *(*name_fn)(void);
    int (*run_fn)(void);
} iokit_test_case;

static iokit_test_case iokit_tests[] = {
    { test_name_iokit_enumerate,  test_run_iokit_enumerate  },
    { test_name_iokit_properties, test_run_iokit_properties },
};

static const size_t iokit_test_count = sizeof(iokit_tests) / sizeof(iokit_tests[0]);

int iokit_run_all(void) {
    printf("[IOKIT] running %zu tests\n", iokit_test_count);
    size_t i;
    int failures = 0;

    for (i = 0; i < iokit_test_count; i++) {
        const char *name = iokit_tests[i].name_fn();
        printf("[IOKIT %zu/%zu] %s \n", i + 1, iokit_test_count, name);
        int ret = iokit_tests[i].run_fn();
        if (ret == 0) {
            printf("[IOKIT PASS] %s\n\n", name);
        } else {
            printf("[IOKIT FAIL] %s (code=%d)\n\n", name, ret);
            failures++;
        }
    }

    printf("[IOKIT] Done. Failures: %d\n", failures);
    return failures == 0 ? 0 : 1;
}
