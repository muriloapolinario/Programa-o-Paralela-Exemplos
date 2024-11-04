#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void* task1(void* arg) {
    pthread_mutex_lock(&lock1);
    printf("Task 1 acquired lock1\n");
    sleep(1);

    printf("Task 1 waiting for lock2\n");
    pthread_mutex_lock(&lock2);
    printf("Task 1 acquired lock2\n");

    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);

    return NULL;
}

void* task2(void* arg) {
    pthread_mutex_lock(&lock2);
    printf("Task 2 acquired lock2\n");
    sleep(1);

    printf("Task 2 waiting for lock1\n");
    pthread_mutex_lock(&lock1);
    printf("Task 2 acquired lock1\n");

    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);

    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, task1, NULL);
    pthread_create(&t2, NULL, task2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    return 0;
}

// Neste exemplo, ilustramos um deadlock onde duas threads tentam adquirir dois mutexes em uma ordem oposta, resultando em uma espera circular.