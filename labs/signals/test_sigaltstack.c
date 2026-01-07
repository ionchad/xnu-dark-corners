#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

const char *test_name_sigaltstack(void) {
    return "sigaltstack";
}

static void handler(int sig) {
    stack_t ss;
    if (sigaltstack(NULL, &ss) == 0) {
        if (ss.ss_flags & SS_ONSTACK) {
            printf("[sigaltstack] handler running on alternate stack\n");
        } else {
            printf("[sigaltstack] handler running on normal stack\n");
        }
    }
}

int test_run_sigaltstack(void) {
    size_t stack_size = SIGSTKSZ;
    void *stack = malloc(stack_size);
    if (!stack) {
        perror("malloc");
        return -1;
    }

    stack_t ss;
    ss.ss_sp = stack;
    ss.ss_size = stack_size;
    ss.ss_flags = 0;

    if (sigaltstack(&ss, NULL) != 0) {
        perror("sigaltstack");
        free(stack);
        return -1;
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sa.sa_flags = SA_ONSTACK;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) != 0) {
        perror("sigaction");
        free(stack);
        return -1;
    }

    printf("[sigaltstack] sending SIGUSR1 to self\n");
    raise(SIGUSR1);

    free(stack);
    return 0;
}
