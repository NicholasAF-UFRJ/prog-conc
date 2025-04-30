#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

int nThreads;
long long int atual = 0, total_primos = 0, N;

pthread_mutex_t mutex_atual, mutex_total; //Mutexes para proteger o número atual e o total de primos

int ehPrimo(long long int n) { //Função provida pela professora, que verifica se um número é primo
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (long long int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void* numPrimos(void* arg) {
    while (1) {
        long long int num;

        pthread_mutex_lock(&mutex_atual);
        if (atual > N) { //Se o número a ser testado passou do limite, sai do loop
            pthread_mutex_unlock(&mutex_atual);
            break;
        }
        num = atual++; //Pega o número atual e incrementa
        pthread_mutex_unlock(&mutex_atual);

        if (ehPrimo(num)) { //Se o número for primo, incrementa o total de primos       
            pthread_mutex_lock(&mutex_total);
            total_primos++; 
            pthread_mutex_unlock(&mutex_total);
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    double inicio, fim, delta;

    if (argc != 3) {
        printf("Uso: %s <N> <nThreads>\n", argv[0]);
        return 1;
    }

    N = atoll(argv[1]); 
    nThreads = atoi(argv[2]);

    pthread_t threads[nThreads];

    GET_TIME(inicio);

    for (int i = 0; i < nThreads; i++) { //Cria as threads
        pthread_create(&threads[i], NULL, numPrimos, NULL);
    }

    for (int i = 0; i < nThreads; i++) { //Espera as threads terminarem
        pthread_join(threads[i], NULL);
    }

    GET_TIME(fim);

    delta = fim - inicio;

    printf("Total de primos entre 1 e %lld: %lld\n", N, total_primos);
    printf("Tempo de execucao: %.6f segundos\n", delta);

    return 0;
}

