#include <stdio.h>
#include <stdlib.h>


const char *test_name_zombie_wait4(void);
int test_run_zombie_wait4(void);

const char *test_name_waitid_weird(void);
int test_run_waitid_weird(void);

const char *test_name_setsid_pgrp(void);
int test_run_setsid_pgrp(void);

typedef struct {
    const char *(*name_fn)(void);
    int (*run_fn)(void);
} proc_test_case;

static proc_test_case proc_tests[] = {
    { test_name_zombie_wait4,    test_run_zombie_wait4    },
    { test_name_waitid_weird,    test_run_waitid_weird    },
    { test_name_setsid_pgrp,     test_run_setsid_pgrp     },
};

static const size_t proc_test_count =
    sizeof(proc_tests) / sizeof(proc_tests[0]);

int proc_run_all(void) {
    printf("[PROC] running %zu tests\n", proc_test_count);
    size_t i;
    int failures = 0;

    for (i = 0; i < proc_test_count; i++) {
        const char *name = proc_tests[i].name_fn();
        printf("[PROC %zu/%zu] %s \n",
               i + 1, proc_test_count, name);
        int ret = proc_tests[i].run_fn();
        if (ret == 0) {
            printf("[PROC PASS] %s\n\n", name);
        } else {
            printf("[PROC FAIL] %s (code=%d)\n\n", name, ret);
            failures++;
        }
    }

    printf("[PROC] Done. Failures: %d\n", failures);
    return failures == 0 ? 0 : 1;
}
