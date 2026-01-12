#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

const char *test_name_cond_spurious_wakeup(void) {
    return "cond_spurious_wakeup";
}

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int ready = 0;
static int spurious_count = 0;

static void *waiter_thread(void *arg) {
    (void)arg;
    pthread_mutex_lock(&mutex);
    
    while (!ready) {
        printf("[cond_spurious_wakeup] Waiter: waiting on condition\n");
        pthread_cond_wait(&cond, &mutex);
        
        if (!ready) {
            spurious_count++;
            printf("[cond_spurious_wakeup] Waiter: spurious wakeup detected!\n");
        }
    }
    
    printf("[cond_spurious_wakeup] Waiter: condition met, exiting\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int test_run_cond_spurious_wakeup(void) {
    printf("[cond_spurious_wakeup] Testing condition variable spurious wakeups\n");

    pthread_t thread;
    if (pthread_create(&thread, NULL, waiter_thread, NULL) != 0) {
        perror("pthread_create");
        return -1;
    }

    sleep(1);


    pthread_cond_signal(&cond);
    usleep(100000);

    
    pthread_mutex_lock(&mutex);
    ready = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    pthread_join(thread, NULL);

    printf("[cond_spurious_wakeup] Spurious wakeups detected: %d\n", spurious_count);
    return 0;
}
