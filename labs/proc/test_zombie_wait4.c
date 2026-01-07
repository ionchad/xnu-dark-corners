#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

const char *test_name_zombie_wait4(void) {
    return "zombie_wait4";
}


int test_run_zombie_wait4(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        _exit(42);
    }

    sleep(3);
    printf("[zombie_wait4] Parent: child PID=%d exited 3s ago, checking zombie status\n", pid);

    int status = 0;
    pid_t w = wait4(pid, &status, WNOHANG, NULL);
    if (w == 0) {
        printf("[zombie_wait4] wait4(WNOHANG): child still zombie\n");
    } else if (w == pid) {
        printf("[zombie_wait4] wait4: reaped child, status=0x%x\n", status);
        if (WIFEXITED(status)) {
            printf("[zombie_wait4] confirmed exit code %d\n", WEXITSTATUS(status));
        }
    } else {
        perror("wait4");
        return -1;
    }

    w = wait4(pid, &status, 0, NULL);
    if (w == pid) {
        printf("[zombie_wait4] final wait4: reaped child cleanly\n");
    } else {
        perror("final wait4");
        return -1;
    }

    return 0;
}
