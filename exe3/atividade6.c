/* Aluno: Nicholas de Araujo Figueira DRE: 121088218 */

/*
    Arquivo para calcular o número de primos em um buffer 
    compartilhado usando threads produtoras e consumidoras.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

int N, M, C; // N: número de inteiros, M: tamanho do buffer, C: número de threads consumidoras
int *Buffer;
int totalPrimos = 0;
int *contadorPrimos;

sem_t slotCheio, slotVazio, mutex;

// Função para verificar se um número é primo
int ehPrimo(long long int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Função do produtor
void* produtor(void* arg) {
    static int in = 0;
    // Produz N números inteiros e os coloca no buffer
    for (int i = 0; i < N; i++) {
        sem_wait(&slotVazio);
        sem_wait(&mutex);

        Buffer[in] = i;
        in = (in + 1) % M;

        sem_post(&mutex);
        sem_post(&slotCheio);
    }

    // Sinaliza o fim da produção para os consumidores
    for (int i = 0; i < C; i++) {
        sem_wait(&slotVazio);
        sem_wait(&mutex);
        Buffer[in] = -1;
        in = (in + 1) % M;
        sem_post(&mutex);
        sem_post(&slotCheio);
    }

    pthread_exit(NULL);
}

// Função do consumidor
void* consumidor(void* arg) {
    int id = *(int*)arg;

    static int out = 0;
    // Consumidor processa os números do buffer
    while (1) {
        // Espera por um número no buffer
        sem_wait(&slotCheio);
        sem_wait(&mutex);

        int valor = Buffer[out];
        out = (out + 1) % M;

        sem_post(&mutex);
        sem_post(&slotVazio);

        // Se o valor for -1, significa que não há mais números para processar
        if (valor == -1){
            break;
        }
        // Verifica se o número é primo
        if (ehPrimo(valor)) {
            sem_wait(&mutex);
            contadorPrimos[id]++;
            totalPrimos++;
            sem_post(&mutex);
        }
    }
    free(arg);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 4){
        printf("Erro: <nome do programa> <número de primos - N> <tamanho do buffer - M> <número de consumidores - C>\n");
        return 1;
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    C = atoi(argv[3]);

    Buffer = malloc(sizeof(int) * M);
    contadorPrimos = malloc(C * sizeof(int));
    if (!contadorPrimos) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        return 1;
    }
    for (int i = 0; i < C; i++) {
        contadorPrimos[i] = 0;
    }

    if (Buffer == NULL || contadorPrimos == NULL) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        return 1;
    }

    sem_init(&slotCheio, 0, 0);
    sem_init(&slotVazio, 0, M);
    sem_init(&mutex, 0, 1);

    pthread_t produtor_t;
    pthread_t consumidores_t[C];

    pthread_create(&produtor_t, NULL, produtor, NULL);

    for (int i = 0; i < C; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&consumidores_t[i], NULL, consumidor, id);
    }

    pthread_join(produtor_t, NULL);
    for (int i = 0; i < C; i++) {
        pthread_join(consumidores_t[i], NULL);
    }

    int vencedora = 0;
    for (int i = 1; i < C; i++) {
        if (contadorPrimos[i] > contadorPrimos[vencedora])
            vencedora = i;
    }
    //Trata-se do consumidor que encontrou mais números primos
    //Considerando também o caso de empate
    int maxPrimos = 0;
    for (int i = 0; i < C; i++) {
        if (contadorPrimos[i] > maxPrimos)
            maxPrimos = contadorPrimos[i];
    }

    printf("\nResumo das threads consumidoras:\n");
    for (int i = 0; i < C; i++) {
        printf("Consumidor %d encontrou %d primos\n", i, contadorPrimos[i]);
    }

    printf("\nTotal de números primos encontrados: %d\n", totalPrimos);
    for (int i = 0; i < C; i++) {
        if (contadorPrimos[i] == maxPrimos) {
            printf(" - Consumidor %d\n", i);
        }
    }

    sem_destroy(&slotCheio);
    sem_destroy(&slotVazio);
    sem_destroy(&mutex);
    free(Buffer);
    free(contadorPrimos);

    return 0;
}