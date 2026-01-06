#include <stdio.h>
#include <stdlib.h>


const char *test_name_ptrace_deny_attach(void);
int test_run_ptrace_deny_attach(void);

const char *test_name_ptrace_trace_me(void);
int test_run_ptrace_trace_me(void);

typedef struct {
    const char *(*name_fn)(void);
    int (*run_fn)(void);
} ptrace_test_case;

static ptrace_test_case ptrace_tests[] = {
    { test_name_ptrace_deny_attach, test_run_ptrace_deny_attach },
    { test_name_ptrace_trace_me,    test_run_ptrace_trace_me    },
};

static const size_t ptrace_test_count =
    sizeof(ptrace_tests) / sizeof(ptrace_tests[0]);

int ptrace_run_all(void) {
    printf("[PTRACE] running %zu tests\n", ptrace_test_count);
    size_t i;
    int failures = 0;

    for (i = 0; i < ptrace_test_count; i++) {
        const char *name = ptrace_tests[i].name_fn();
        printf("[PTRACE %zu/%zu] %s \n",
               i + 1, ptrace_test_count, name);
        int ret = ptrace_tests[i].run_fn();
        if (ret == 0) {
            printf("[PTRACE PASS] %s\n\n", name);
        } else {
            printf("[PTRACE FAIL] %s (code=%d)\n\n", name, ret);
            failures++;
        }
    }

    printf("[PTRACE] Done. Failures: %d\n", failures);
    return failures == 0 ? 0 : 1;
}
