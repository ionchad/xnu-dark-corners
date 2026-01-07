#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

const char *test_name_waitid_weird(void) {
    return "waitid_weird";
}

int test_run_waitid_weird(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        printf("[waitid_weird] Child PID=%d exiting with code 123\n", getpid());
        _exit(123);
        return 0; 
    }


    siginfo_t si = {0};
    
    printf("[waitid_weird] Testing waitid(P_ALL, WEXITED | WNOWAIT)\n");
    int r = waitid(P_ALL, 0, &si, WEXITED | WNOWAIT);
    if (r == 0) {
        printf("[waitid_weird] WEXITED|WNOWAIT: si_code=%d si_status=%d\n",
               si.si_code, si.si_status);
    } else {
        printf("[waitid_weird] WEXITED|WNOWAIT errno=%d (%s)\n",
               errno, strerror(errno));
    }

    printf("[waitid_weird] Testing waitid(P_PID, pid=%d, WEXITED)\n", pid);
    r = waitid(P_PID, pid, &si, WEXITED);
    if (r == 0) {
        printf("[waitid_weird] P_PID WEXITED: si_code=%d si_status=%d\n",
               si.si_code, si.si_status);
    } else {
        printf("[waitid_weird] P_PID WEXITED errno=%d (%s)\n",
               errno, strerror(errno));
    }


    int status;
    waitpid(pid, &status, WNOHANG);
    return 0;
}
