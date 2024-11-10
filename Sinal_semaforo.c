#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SEMAFORO 3
pthread_mutex_t mutex_semaforo = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_semaforo= PTHREAD_COND_INITIALIZER;
int contador_semaforo =MAX_SEMAFORO;

void espera_semaforo() {
    pthread_mutex_lock(&mutex_semaforo);
    while (contador_semaforo == 0) {
        pthread_cond_wait(&cond_semaforo, &mutex_semaforo);
    }
    contador_semaforo--;
    pthread_mutex_unlock(&mutex_semaforo);
}

void sinaliza_semaforo() {
    pthread_mutex_lock(&mutex_semaforo);
    contador_semaforo++;
    pthread_cond_signal(&cond_semaforo);
    pthread_mutex_unlock(&mutex_semaforo);
}

void *trabalhador(void *arg) {
    int id= *(int *)arg;
    espera_semaforo();
    printf("Thread %d acessou o recurso.\n", id);
    sleep(2);
    printf("Thread %d liberou o recurso.\n", id);
    sinaliza_semaforo();
    return NULL;
}

int main() {
    pthread_t threads[5];
    int ids[5];

    for (int i= 0; i < 5; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, trabalhador,&ids[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

//Este exemplo simula um recurso limitado que permite apenas um número fixo de threads (clientes) acessando ao mesmo tempo, controlando a entrada e saída de threads para evitar que mais threads do que o permitido acessem o recurso simultaneamente.
