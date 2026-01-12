#include <stdio.h>
#include <stdlib.h>

const char *test_name_cache_timing(void);
int test_run_cache_timing(void);

const char *test_name_flush_reload(void);
int test_run_flush_reload(void);

const char *test_name_prime_probe(void);
int test_run_prime_probe(void);

typedef struct {
    const char *(*name_fn)(void);
    int (*run_fn)(void);
} side_channels_test_case;

static side_channels_test_case side_channels_tests[] = {
    { test_name_cache_timing,  test_run_cache_timing  },
    { test_name_flush_reload,  test_run_flush_reload  },
    { test_name_prime_probe,   test_run_prime_probe   },
};

static const size_t side_channels_test_count = sizeof(side_channels_tests) / sizeof(side_channels_tests[0]);

int side_channels_run_all(void) {
    printf("[SIDE-CHANNELS] running %zu tests\n", side_channels_test_count);
    size_t i;
    int failures = 0;

    for (i = 0; i < side_channels_test_count; i++) {
        const char *name = side_channels_tests[i].name_fn();
        printf("[SIDE-CHANNELS %zu/%zu] %s \n", i + 1, side_channels_test_count, name);
        int ret = side_channels_tests[i].run_fn();
        if (ret == 0) {
            printf("[SIDE-CHANNELS PASS] %s\n\n", name);
        } else {
            printf("[SIDE-CHANNELS FAIL] %s (code=%d)\n\n", name, ret);
            failures++;
        }
    }

    printf("[SIDE-CHANNELS] Done. Failures: %d\n", failures);
    return failures == 0 ? 0 : 1;
}
