#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t resource_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t readers_proceed = PTHREAD_COND_INITIALIZER;
pthread_cond_t writer_proceed = PTHREAD_COND_INITIALIZER;

int read_count = 0;
int shared_data = 0;

void *reader(void *arg) {
    int reader_id = *((int *)arg);

    pthread_mutex_lock(&resource_mutex);
    read_count++;
    pthread_mutex_unlock(&resource_mutex);

    printf("Reader %d: read shared_data = %d\n", reader_id, shared_data);
    sleep(1);

    pthread_mutex_lock(&resource_mutex);
    read_count--;
    if (read_count == 0) {
        pthread_cond_signal(&writer_proceed);
    }
    pthread_mutex_unlock(&resource_mutex);

    return NULL;
}

void *writer(void *arg) {
    int writer_id = *((int *)arg);

    pthread_mutex_lock(&resource_mutex);
    shared_data++;
    printf("Writer %d: updated shared_data to %d\n", writer_id, shared_data);
    sleep(1);
    pthread_mutex_unlock(&resource_mutex);

    pthread_cond_signal(&readers_proceed);
    return NULL;
}

int main() {
    pthread_t readers[5], writers[3];

    for (int i = 0; i < 5; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&readers[i], NULL, reader, id);
    }
    for (int i = 0; i < 3; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&writers[i], NULL, writer, id);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(writers[i], NULL);
    }

    pthread_mutex_destroy(&resource_mutex);
    pthread_cond_destroy(&readers_proceed);
    pthread_cond_destroy(&writer_proceed);

    return 0;
}

// Neste exemplo, é usado o padrão de leitores e escritores para permitir que várias threads leiam um recurso compartilhado simultaneamente,
// enquanto uma thread escritora tem acesso exclusivo.