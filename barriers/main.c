#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5

pthread_barrier_t barrier;

void *thread_task(void *arg) {
    int id = *((int *)arg);
    printf("Thread %d: Executando a Fase 1\n", id);
    sleep(1);

    printf("Thread %d: Aguardando na barreira após a Fase 1\n", id);
    pthread_barrier_wait(&barrier);

    printf("Thread %d: Iniciando a Fase 2\n", id);
    sleep(1);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_task, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);

    printf("Todas as threads completaram as duas fases.\n");
    return 0;
}