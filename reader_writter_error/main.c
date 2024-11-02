#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int shared_data = 0;

void *reader(void *arg) {
    int id = *((int *)arg);
    printf("Leitor %d: lê shared_data = %d\n", id, shared_data);
    return NULL;
}

void *writer(void *arg) {
    int id = *((int *)arg);
    shared_data += 1;
    printf("Escritor %d: atualiza shared_data para %d\n", id, shared_data);
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

    return 0;
}

//APESAR DO PROGRAMA RODAR PERFEITAMENTE, EXITE UMA VULNERABILIDADE NELE.
//NA LINHA 15 DO CÓDIGO, EXITE UMA REGIÃO SEM SINCRONIZAÇÃO, O QUE CONFIGURA
// UMA CONDIÇÃO DE CORRIDA, FAZENDO COM QUE O PROGRAMA SEJA "RESOLVIDO" APENAS
//PELA SINCRONIA OU ORDEM DOS EVENTOS.