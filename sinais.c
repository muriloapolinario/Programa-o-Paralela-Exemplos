#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
int ready = 0;

void* wait_for_signal(void* arg) {
    printf("Thread esperando por sinal\n");

    pthread_mutex_lock(&mutex);
    while (!ready) {
        pthread_cond_wait(&condition, &mutex);
    }
    printf("Thread recebeu o sinal\n");
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* send_signal(void* arg) {
    sleep(2); 
    printf("enviando sinal para aguardar thread\n");

    pthread_mutex_lock(&mutex);
    ready = 1;
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, wait_for_signal, NULL);
    pthread_create(&t2, NULL, send_signal, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);

    return 0;
}

// Neste exemplo, usaremos sinais para comunicar um evento entre threads.
// Uma thread enviará um sinal (pthread_cond_signal) para notificar outra thread que está esperando por uma condição específica.