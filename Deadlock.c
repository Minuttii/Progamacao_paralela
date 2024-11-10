#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t recurso1= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t recurso2 = PTHREAD_MUTEX_INITIALIZER;

void *thread1(void *arg) {
    pthread_mutex_lock(&recurso1);
    printf("Thread 1 bloqueou o recurso 1.\n");
    sleep(1);
    pthread_mutex_lock(&recurso2);
    printf("Thread 1 bloqueou o recurso 2.\n");

    pthread_mutex_unlock(&recurso2);
    pthread_mutex_unlock(&recurso1);

    return NULL;
}

void *thread2(void *arg) {
    pthread_mutex_lock(&recurso2);
    printf("Thread 2 bloqueou o recurso 2.\n");
    sleep(1);
    pthread_mutex_lock(&recurso1);
    printf("Thread 2 bloqueou o recurso 1.\n");

    pthread_mutex_unlock(&recurso1);
    pthread_mutex_unlock(&recurso2);

    return NULL;
}

int main() {
    pthread_t t1,t2;

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

//O programa cria duas threads que bloqueiam os recursos em ordem oposta. Assim, cada thread bloqueia o recurso que a outra precisa, resultando em um estado de deadlock, exemplificando a necessidade de estratégias para evitar deadlocks em sistemas de múltiplos recursos.
