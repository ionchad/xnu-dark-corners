#include <stdio.h>
#include <stdlib.h>

const char *test_name_mutex_race(void);
int test_run_mutex_race(void);

const char *test_name_cond_spurious_wakeup(void);
int test_run_cond_spurious_wakeup(void);

const char *test_name_pthread_cancel(void);
int test_run_pthread_cancel(void);

typedef struct {
    const char *(*name_fn)(void);
    int (*run_fn)(void);
} thread_races_test_case;

static thread_races_test_case thread_races_tests[] = {
    { test_name_mutex_race,           test_run_mutex_race           },
    { test_name_cond_spurious_wakeup, test_run_cond_spurious_wakeup },
    { test_name_pthread_cancel,       test_run_pthread_cancel       },
};

static const size_t thread_races_test_count = sizeof(thread_races_tests) / sizeof(thread_races_tests[0]);

int thread_races_run_all(void) {
    printf("[THREAD-RACES] running %zu tests\n", thread_races_test_count);
    size_t i;
    int failures = 0;

    for (i = 0; i < thread_races_test_count; i++) {
        const char *name = thread_races_tests[i].name_fn();
        printf("[THREAD-RACES %zu/%zu] %s \n", i + 1, thread_races_test_count, name);
        int ret = thread_races_tests[i].run_fn();
        if (ret == 0) {
            printf("[THREAD-RACES PASS] %s\n\n", name);
        } else {
            printf("[THREAD-RACES FAIL] %s (code=%d)\n\n", name, ret);
            failures++;
        }
    }

    printf("[THREAD-RACES] Done. Failures: %d\n", failures);
    return failures == 0 ? 0 : 1;
}
