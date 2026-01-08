#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/syscall.h>
#include <signal.h>
#include <setjmp.h>

static sigjmp_buf jmp_env;
static volatile sig_atomic_t got_sigsys = 0;

static void sigsys_handler(int sig) {
    (void)sig;
    got_sigsys = 1;
    siglongjmp(jmp_env, 1);
}

const char *test_name_unknown_syscall(void) {
    return "unknown_syscall";
}

int test_run_unknown_syscall(void) {
    struct sigaction sa, old_sa;
    sa.sa_handler = sigsys_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSYS, &sa, &old_sa);

    int test_numbers[] = { 999, 1000, 2000, 5000 };
    size_t i;

    printf("[unknown_syscall] Probing unknown syscall numbers\n");

    for (i = 0; i < sizeof(test_numbers) / sizeof(test_numbers[0]); i++) {
        int num = test_numbers[i];
        errno = 0;
        got_sigsys = 0;

        if (sigsetjmp(jmp_env, 1) == 0) {
            long ret = syscall(num);
            int e = errno;
            printf("[unknown_syscall] syscall(%d): ret=%ld errno=%d (%s)\n",
                   num, ret, e, strerror(e));
        } else {
            printf("[unknown_syscall] syscall(%d): caught SIGSYS (invalid syscall)\n", num);
        }
    }

    sigaction(SIGSYS, &old_sa, NULL);
    return 0;
}
