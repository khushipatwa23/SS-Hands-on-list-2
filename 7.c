/* Program to create three threads and print their thread IDs */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg) {
    int id = *(int *)arg;

    printf("Thread %d ID: %lu\n",
           id, (unsigned long)pthread_self());

    return NULL;
}

int main() {
    pthread_t threads[3];
    int ids[3];

    for (int i = 0; i < 3; i++) {
        ids[i] = i + 1;

        if (pthread_create(&threads[i], NULL,
                           thread_function, &ids[i]) != 0) {
            fprintf(stderr, "Thread creation failed\n");
            return 1;
        }
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
