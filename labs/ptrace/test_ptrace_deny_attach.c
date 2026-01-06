#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#ifndef PT_DENY_ATTACH
#define PT_DENY_ATTACH 31
#endif

const char *test_name_ptrace_deny_attach(void) {
    return "ptrace_deny_attach";
}

int test_run_ptrace_deny_attach(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        printf("[ptrace_deny_attach] Child PID=%d calling PT_DENY_ATTACH\n",
               getpid());

        int r = ptrace(PT_DENY_ATTACH, 0, 0, 0);
        if (r != 0) {
            int e = errno;
            printf("[ptrace_deny_attach] Child: PT_DENY_ATTACH failed errno=%d (%s)\n",
                   e, strerror(e));
            _exit(1);
        }

        printf("[ptrace_deny_attach] Child: PT_DENY_ATTACH succeeded, looping...\n");
        for (;;) {
            sleep(1);
        }
    }

    sleep(1); 

    printf("[ptrace_deny_attach] Parent: trying PT_ATTACHEXC to child PID=%d\n", pid);
#ifdef PT_ATTACHEXC
    int r = ptrace(PT_ATTACHEXC, pid, (caddr_t)1, 0);
#else
    int r = ptrace(PT_ATTACH, pid, 0, 0);
#endif
    if (r != 0) {
        int e = errno;
        printf("[ptrace_deny_attach] Parent: PT_ATTACH failed errno=%d (%s)\n",
               e, strerror(e));
    } else {
        printf("[ptrace_deny_attach] Parent: PT_ATTACH succeeded (unexpected)\n");
    }

    int status = 0;
    pid_t w = waitpid(pid, &status, WNOHANG);
    if (w == 0) {
        printf("[ptrace_deny_attach] Parent: child still running\n");
    } else if (w == pid) {
        if (WIFSIGNALED(status)) {
            printf("[ptrace_deny_attach] Parent: child died from signal %d\n",
                   WTERMSIG(status));
        } else if (WIFEXITED(status)) {
            printf("[ptrace_deny_attach] Parent: child exited with code %d\n",
                   WEXITSTATUS(status));
        } else {
            printf("[ptrace_deny_attach] Parent: child changed state (status=0x%x)\n",
                   status);
        }
    } else {
        perror("waitpid");
    }

    kill(pid, SIGKILL);
    waitpid(pid, NULL, 0);
    return 0;
}
