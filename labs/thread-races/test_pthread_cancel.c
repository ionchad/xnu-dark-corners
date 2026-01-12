#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

const char *test_name_pthread_cancel(void) {
    return "pthread_cancel";
}

static void cleanup_handler(void *arg) {
    printf("[pthread_cancel] Cleanup handler called with arg=%s\n", (char *)arg);
}

static void *cancelable_thread(void *arg) {
    (void)arg;
    
    pthread_cleanup_push(cleanup_handler, "test_data");
    
    printf("[pthread_cancel] Thread: entering infinite loop\n");
    
    int old_state;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_state);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    
    int i = 0;
    while (1) {
        pthread_testcancel();  
        i++;
        if (i % 1000000 == 0) {
            printf("[pthread_cancel] Thread: still running (iter %d)\n", i);
        }
    }
    
    pthread_cleanup_pop(1);
    return NULL;
}

int test_run_pthread_cancel(void) {
    printf("[pthread_cancel] Testing pthread cancellation\n");

    pthread_t thread;
    if (pthread_create(&thread, NULL, cancelable_thread, NULL) != 0) {
        perror("pthread_create");
        return -1;
    }

    sleep(1);

    printf("[pthread_cancel] Main: sending cancel to thread\n");
    if (pthread_cancel(thread) != 0) {
        perror("pthread_cancel");
        return -1;
    }

    void *retval;
    if (pthread_join(thread, &retval) != 0) {
        perror("pthread_join");
        return -1;
    }

    if (retval == PTHREAD_CANCELED) {
        printf("[pthread_cancel] Thread was successfully canceled\n");
    } else {
        printf("[pthread_cancel] Thread exited normally (unexpected)\n");
        return -1;
    }

    return 0;
}
