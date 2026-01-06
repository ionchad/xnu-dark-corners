#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#ifndef PT_TRACE_ME
#define PT_TRACE_ME 0
#endif

const char *test_name_ptrace_trace_me(void) {
    return "ptrace_trace_me";
}


int test_run_ptrace_trace_me(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        printf("[ptrace_trace_me] Child PID=%d calling PT_TRACE_ME\n", getpid());
        int r = ptrace(PT_TRACE_ME, 0, 0, 0);
        if (r != 0) {
            int e = errno;
            printf("[ptrace_trace_me] Child: PT_TRACE_ME failed errno=%d (%s)\n",
                   e, strerror(e));
            _exit(1);
        }

        printf("[ptrace_trace_me] Child: PT_TRACE_ME succeeded, raising SIGSTOP\n");
        raise(SIGSTOP);

        printf("[ptrace_trace_me] Child: continuing after SIGSTOP, exiting 0\n");
        _exit(0);
    }


    int status = 0;
    pid_t w;

    printf("[ptrace_trace_me] Parent: waiting for child PID=%d\n", pid);

    w = waitpid(pid, &status, 0);
    if (w < 0) {
        perror("waitpid");
        return -1;
    }

    if (WIFSTOPPED(status)) {
        printf("[ptrace_trace_me] Parent: child stopped by signal %d\n",
               WSTOPSIG(status));
    } else if (WIFEXITED(status)) {
        printf("[ptrace_trace_me] Parent: child exited with code %d\n",
               WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("[ptrace_trace_me] Parent: child terminated by signal %d\n",
               WTERMSIG(status));
    } else {
        printf("[ptrace_trace_me] Parent: child status=0x%x\n", status);
    }

    return 0;
}
