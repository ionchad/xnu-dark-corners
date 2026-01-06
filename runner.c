#include <stdio.h>
#include <stdlib.h>


int vm_run_all(void);

int main(void) {
    int failures = 0;

    printf("xnu-dark-corners: starting all labs\n\n");

    printf("[LAB] VM\n");
    if (vm_run_all() != 0) {
        failures++;
    }

    

    printf("xnu-dark-corners: done. Lab failures: %d\n", failures);
    return failures == 0 ? 0 : 1;
}
