#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

const char *test_name_mutex_race(void) {
    return "mutex_race";
}

static int shared_counter = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *thread_func(void *arg) {
    (void)arg;
    int i;
    for (i = 0; i < 10000; i++) {
        pthread_mutex_lock(&mutex);
        shared_counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int test_run_mutex_race(void) {
    printf("[mutex_race] Starting 4 threads incrementing shared counter\n");

    pthread_t threads[4];
    size_t i;

    shared_counter = 0;

    for (i = 0; i < 4; i++) {
        if (pthread_create(&threads[i], NULL, thread_func, NULL) != 0) {
            perror("pthread_create");
            return -1;
        }
    }

    for (i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("[mutex_race] Final counter value: %d (expected 40000)\n", shared_counter);

    if (shared_counter == 40000) {
        printf("[mutex_race] No race detected (mutex worked)\n");
        return 0;
    } else {
        printf("[mutex_race] Race detected! Counter is wrong\n");
        return -1;
    }
}
