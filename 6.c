/* Program to create three threads using pthreads */

#include <stdio.h>
#include <pthread.h>

void *thread_function(void *arg) {
    return NULL;
}

int main() {
    pthread_t threads[3];

    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL,
                           thread_function, NULL) != 0) {
            fprintf(stderr, "Thread creation failed\n");
            return 1;
        }

        printf("Thread %d created successfully\n", i + 1);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads completed\n");

    return 0;
}
