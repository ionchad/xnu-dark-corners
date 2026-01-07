#include <stdio.h>
#include <stdlib.h>

const char *test_name_sigaltstack(void);
int test_run_sigaltstack(void);

const char *test_name_sigsuspend_race(void);
int test_run_sigsuspend_race(void);

typedef struct {
    const char *(*name_fn)(void);
    int (*run_fn)(void);
} signals_test_case;

static signals_test_case signals_tests[] = {
    { test_name_sigaltstack,     test_run_sigaltstack     },
    { test_name_sigsuspend_race, test_run_sigsuspend_race },
};

static const size_t signals_test_count = sizeof(signals_tests) / sizeof(signals_tests[0]);

int signals_run_all(void) {
    printf("[SIGNALS] running %zu tests\n", signals_test_count);
    size_t i;
    int failures = 0;

    for (i = 0; i < signals_test_count; i++) {
        const char *name = signals_tests[i].name_fn();
        printf("[SIGNALS %zu/%zu] %s \n", i + 1, signals_test_count, name);
        int ret = signals_tests[i].run_fn();
        if (ret == 0) {
            printf("[SIGNALS PASS] %s\n\n", name);
        } else {
            printf("[SIGNALS FAIL] %s (code=%d)\n\n", name, ret);
            failures++;
        }
    }

    printf("[SIGNALS] Done. Failures: %d\n", failures);
    return failures == 0 ? 0 : 1;
}
