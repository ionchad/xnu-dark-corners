#include <stdio.h>
#include <stdlib.h>


int vm_run_all(void);
int ptrace_run_all(void);
int proc_run_all(void);
int signals_run_all(void);

int main(void) {
    int failures = 0;

    printf("xnu-dark-corners: starting all labs\n\n");

    printf("\n [LAB] VM \n");
    if (vm_run_all() != 0) {
        failures++;
    }

    printf("\n [LAB] PTRACE \n");
    if (ptrace_run_all() != 0) {
        failures++;
    }

    printf("\n [LAB] PROC \n");
    if (proc_run_all() != 0) {
        failures++;
}

    printf("\n [LAB] SIGNALS \n");
    if (signals_run_all() != 0) {
        failures++;
}

    printf("\nxnu-dark-corners: done. Lab failures: %d\n", failures);
    return failures == 0 ? 0 : 1;
}
