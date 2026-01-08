#include <stdio.h>
#include <sys/wait.h>
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

const char *test_name_syscall_boundary(void) {
    return "syscall_boundary";
}

int test_run_syscall_boundary(void) {
    struct sigaction sa, old_sa;
    sa.sa_handler = sigsys_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSYS, &sa, &old_sa);

    printf("[syscall_boundary] Testing boundary syscall numbers\n");

    int boundaries[] = { -1, 500, 600 };
    size_t i;

    for (i = 0; i < sizeof(boundaries) / sizeof(boundaries[0]); i++) {
        int num = boundaries[i];
        errno = 0;
        got_sigsys = 0;

        if (sigsetjmp(jmp_env, 1) == 0) {
            long ret = syscall(num);
            int e = errno;
            printf("[syscall_boundary] syscall(%d): ret=%ld errno=%d (%s)\n",
                   num, ret, e, strerror(e));
        } else {
            printf("[syscall_boundary] syscall(%d): caught SIGSYS\n", num);
        }
    }

    printf("[syscall_boundary] Testing with NULL pointer argument\n");
    errno = 0;
    got_sigsys = 0;

    if (sigsetjmp(jmp_env, 1) == 0) {
        long ret = syscall(SYS_open, (const char *)NULL, 0);
        printf("[syscall_boundary] open(NULL): ret=%ld errno=%d (%s)\n",
               ret, errno, strerror(errno));
    } else {
        printf("[syscall_boundary] open(NULL): caught SIGSYS\n");
    }

    sigaction(SIGSYS, &old_sa, NULL);
    return 0;
}
