#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex_dados = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_leitores = PTHREAD_MUTEX_INITIALIZER;
int contador_leitores = 0;
int dados = 0; // Dado compartilhado

void *escritor(void *arg) {
    int id = *(int *)arg;

    while (1) {
        pthread_mutex_lock(&mutex_dados);
        dados++; // Simula a escrita
        printf("Escritor %d escreveu o valor %d\n", id, dados);
        pthread_mutex_unlock(&mutex_dados);
        sleep(1);
    }
    return NULL;
}

void *leitor(void *arg) {
    int id = *(int *)arg;

    while (1) {
        pthread_mutex_lock(&mutex_leitores);
        contador_leitores++;
        if (contador_leitores == 1) {
            pthread_mutex_lock(&mutex_dados);
        }
        pthread_mutex_unlock(&mutex_leitores);

        // Leitura dos dados
        printf("Leitor %d leu o valor %d\n", id, dados);

        pthread_mutex_lock(&mutex_leitores);
        contador_leitores--;
        if (contador_leitores == 0) {
            pthread_mutex_unlock(&mutex_dados);
        }
        pthread_mutex_unlock(&mutex_leitores);

        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t leitores[5], escritores[2];
    int ids[5] = {1, 2, 3, 4, 5};

    for (int i = 0; i < 5; i++) {
        pthread_create(&leitores[i], NULL, leitor, &ids[i]);
    }
    for (int i = 0; i < 2; i++) {
        pthread_create(&escritores[i], NULL, escritor, &ids[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(leitores[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(escritores[i], NULL);
    }

    return 0;
}
