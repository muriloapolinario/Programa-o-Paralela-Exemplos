#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5

pthread_barrier_t barrier;

void* task(void* arg) {
    int id = *((int *)arg);
    printf("Thread %d esperando pela barreira\n", id);
    
    pthread_barrier_wait(&barrier);
    printf("Thread %d passou pela barreira\n", id);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&threads[i], NULL, task, id);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    return 0;
}

// Este exemplo cria uma barreira para sincronizar várias threads antes de elas continuarem a execução.
