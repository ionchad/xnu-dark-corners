#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

const char *test_name_wait4_basic(void) {
    return "wait4_basic";
}

int test_run_wait4_basic(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        _exit(42);
    }

    int status = 0;
    int ret = wait4(pid, &status, 0, NULL);
    if (ret < 0) {
        perror("wait4");
        return -1;
    }

    if (WIFEXITED(status)) {
        int code = WEXITSTATUS(status);
        printf("[wait4_basic] child exited with code %d\n", code);
    } else {
        printf("[wait4_basic] child did not exit normally\n");
    }

    return 0;
}
