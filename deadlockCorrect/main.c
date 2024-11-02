#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Declaração dos mutexes globais
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void* thread_func1(void* arg) {
    // Tenta bloquear os mutexes em ordem consistente (mutex1 depois mutex2)
    pthread_mutex_lock(&mutex1);
    printf("Thread 1: bloqueou mutex1\n");

    sleep(1); // Simula uma operação que leva tempo

    pthread_mutex_lock(&mutex2);
    printf("Thread 1: bloqueou mutex2\n");

    // Realiza operações críticas
    printf("Thread 1: realizando operações críticas\n");

    // Libera os mutexes na ordem inversa
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);

    pthread_exit(NULL);
}

void* thread_func2(void* arg) {
    // Tenta bloquear os mutexes em ordem consistente (mutex1 depois mutex2)
    pthread_mutex_lock(&mutex1);
    printf("Thread 2: bloqueou mutex1\n");

    sleep(1); // Simula uma operação que leva tempo

    pthread_mutex_lock(&mutex2);
    printf("Thread 2: bloqueou mutex2\n");

    // Realiza operações críticas
    printf("Thread 2: realizando operações críticas\n");

    // Libera os mutexes na ordem inversa
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);

    pthread_exit(NULL);
}

int main() {
    // Inicializa os mutexes
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    // Cria as threads
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread_func1, NULL);
    pthread_create(&thread2, NULL, thread_func2, NULL);

    // Aguarda a finalização das threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Destroi os mutexes
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    printf("Programa finalizado.\n");
    return 0;
}
