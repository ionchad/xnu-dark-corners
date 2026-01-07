#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

const char *test_name_sigsuspend_race(void) {
    return "sigsuspend_race";
}

static volatile sig_atomic_t got_signal = 0;

static void handler(int sig) {
    (void)sig;
    got_signal = 1;
}

int test_run_sigsuspend_race(void) {
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) != 0) {
        perror("sigaction");
        return -1;
    }

    sigset_t block_set, wait_set;
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGUSR1);

    if (sigprocmask(SIG_BLOCK, &block_set, NULL) != 0) {
        perror("sigprocmask");
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        sleep(1);
        printf("[sigsuspend_race] Child: sending SIGUSR1 to parent\n");
        kill(getppid(), SIGUSR1);
        _exit(0);
    }

    sigemptyset(&wait_set);
    printf("[sigsuspend_race] Parent: waiting for signal via sigsuspend\n");
    sigsuspend(&wait_set);

    printf("[sigsuspend_race] Parent: signal received (got_signal=%d)\n", got_signal);

    waitpid(pid, NULL, 0);
    return 0;
}
