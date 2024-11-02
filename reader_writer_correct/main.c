#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int shared_data = 0;
int read_count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t write_mutex = PTHREAD_MUTEX_INITIALIZER;

void *reader(void *arg) {
    int id = *((int *)arg);

    pthread_mutex_lock(&mutex);
    read_count++;
    if (read_count == 1) {
        pthread_mutex_lock(&write_mutex);
    }
    pthread_mutex_unlock(&mutex);

    printf("Leitor %d: lê shared_data = %d\n", id, shared_data);

    pthread_mutex_lock(&mutex);
    read_count--;
    if (read_count == 0) {
        pthread_mutex_unlock(&write_mutex);
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *writer(void *arg) {
    int id = *((int *)arg);

    pthread_mutex_lock(&write_mutex);

    shared_data += 1;
    printf("Escritor %d: atualiza shared_data para %d\n", id, shared_data);

    pthread_mutex_unlock(&write_mutex);
    return NULL;
}

int main() {
    pthread_t readers[5], writers[5];
    int i, ids[5];

    for (i = 0; i < 5; i++) {
        ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &ids[i]);
        pthread_create(&readers[i], NULL, reader, &ids[i]);
    }

    for (i = 0; i < 5; i++) {
        pthread_join(writers[i], NULL);
        pthread_join(readers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&write_mutex);

    return 0;
}