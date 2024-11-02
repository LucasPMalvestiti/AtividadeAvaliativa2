#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define NUM_THREADS 5

volatile sig_atomic_t running = 1;

void signal_handler(int signum) {
    running = 0;
}

void *counting_thread(void *thread_id) {
    long tid = (long)thread_id;
    int counter = 0;

    printf("Thread %ld iniciada.\n", tid);

    while (running) {
        printf("Thread %ld: contador = %d\n", tid, counter++);
        sleep(1);
    }

    printf("Thread %ld finalizando.\n", tid);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    long i;

    signal(SIGINT, signal_handler);

    for (i = 0; i < NUM_THREADS; i++) {
        int rc = pthread_create(&threads[i], NULL, counting_thread, (void *)i);
        if (rc) {
            printf("ERRO: não foi possível criar a thread %ld, código de erro: %d\n", i, rc);
            exit(-1);
        }
    }

    printf("Pressione Ctrl+C para parar as threads.\n");

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Programa finalizado.\n");
    return 0;
}
