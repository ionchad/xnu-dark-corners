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

#ifndef PT_ATTACHEXC
#define PT_ATTACHEXC 14
#endif

const char *test_name_ptrace_deny_attach(void) {
    return "ptrace_deny_attach";
}

static int inner_ptrace_deny_attach(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork(inner)");
        return -1;
    }

    if (pid == 0) {
        printf("[ptrace_deny_attach] Child PID=%d calling PT_DENY_ATTACH\n", getpid());
        int r = ptrace(PT_DENY_ATTACH, 0, 0, 0);
        if (r != 0) {
            int e = errno;
            printf("[ptrace_deny_attach] Child: PT_DENY_ATTACH failed errno=%d (%s)\n", e, strerror(e));
            _exit(1);
        }
        printf("[ptrace_deny_attach] Child: PT_DENY_ATTACH succeeded, looping...\n");
        for (;;) sleep(1);
    }


    sleep(1);
    printf("[ptrace_deny_attach] Parent(inner): trying PT_ATTACHEXC to PID=%d\n", pid);

    int r = ptrace(PT_ATTACHEXC, pid, (caddr_t)1, 0);
    if (r != 0) {
        int e = errno;
        printf("[ptrace_deny_attach] Parent(inner): attach failed errno=%d (%s)\n", e, strerror(e));
    } else {
        printf("[ptrace_deny_attach] Parent(inner): attach succeeded (unexpected)\n");
    }


    kill(pid, SIGKILL);
    int status;
    waitpid(pid, &status, 0);
    
    if (WIFSIGNALED(status)) {
        printf("[ptrace_deny_attach] Parent(inner): child killed by signal %d\n", WTERMSIG(status));
    }

    return 0;
}

int test_run_ptrace_deny_attach(void) {
    pid_t outer = fork();
    if (outer < 0) {
        perror("fork(outer)");
        return -1;
    }
    
    if (outer == 0) {
        int rc = inner_ptrace_deny_attach();
        _exit(rc == 0 ? 0 : 1);
    }

    
    int status;
    waitpid(outer, &status, 0);
    
    if (WIFSIGNALED(status)) {
        printf("[ptrace_deny_attach] Test process died from signal %d (expected on macOS)\n", WTERMSIG(status));
        return 0; 
    } else if (WIFEXITED(status)) {
        int code = WEXITSTATUS(status);
        printf("[ptrace_deny_attach] Test process exited with code %d\n", code);
        return code == 0 ? 0 : -1;
    }

    printf("[ptrace_deny_attach] Test process status=0x%x\n", status);
    return -1;
}
