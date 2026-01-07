#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <grp.h>

const char *test_name_setsid_pgrp(void) {
    return "setsid_pgrp";
}


int test_run_setsid_pgrp(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        pid_t sid = setsid();
        if (sid < 0) {
            perror("setsid");
            _exit(1);
        }

        pid_t pgid = getpgrp();
        printf("[setsid_pgrp] Child: new session leader PID=%d, PGID=%d\n",
               getpid(), pgid);

        for (;;) {
            pause();
        }
    }

    sleep(1);
    printf("[setsid_pgrp] Parent: child PID=%d, sending SIGUSR1\n", pid);

    kill(-pid, SIGUSR1);  

    sleep(1);
    kill(pid, SIGKILL);
    waitpid(pid, NULL, 0);

    printf("[setsid_pgrp] Parent: child cleaned up\n");
    return 0;
}
