#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5
pthread_mutex_t mutex_barreira= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_barreira = PTHREAD_COND_INITIALIZER;
int contador =0;

void barreira() {
    pthread_mutex_lock(&mutex_barreira);
    contador++;
    if (contador ==NUM_THREADS) {
        contador = 0;
        pthread_cond_broadcast(&cond_barreira);
    } else {
        while (pthread_cond_wait(&cond_barreira, &mutex_barreira) !=0);
    }
    pthread_mutex_unlock(&mutex_barreira);
}

void *trabalhador(void *arg) {
    int id = *(int *)arg;
    printf("thread %d atingiu a barreira.\n", id);
    barreira();
    printf("Thread %d passou da barreira.\n", id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, trabalhador, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
//O programa cria uma barreira para sincronizar as threads, de forma que todas precisam alcançar a barreira antes de continuar. A barreira é implementada com mutexes e variáveis de condição, o que garante que todas as threads se sincronizem antes de avançar.
