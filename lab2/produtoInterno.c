/* Aluno: Nicholas de Araujo Figueira DRE: 121088218 */

/*Arquivo para calcular o produto interno de dois vetores gerados usando concorrência e comparando com sequencial */
/* A maior parte desse código foi baseado no código soma_vetor_conc do lab2 */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 
#include <math.h> 

// Variáveis Globais
long int n;
float *vector1, *vector2;
int nthreads;

void *InnerProduct(void *tid) {
    long int id = (long int) tid;
    int ini, end, block;
    double *ret;
    float localInnerProd = 0.0;

    //Não utilizei blocos, mas deixei mesmo assim
    block = n / nthreads; 
    ini = id * block; 
    end = ini + block;
    if (id == nthreads - 1) end = n;

    for (long int i = ini; i < end; i++) { 
        localInnerProd += vector1[i] * vector2[i];
    }

    ret = malloc(sizeof(float));
    if (ret != NULL) *ret = localInnerProd;
    else printf("--ERRO: malloc() thread\n");
    pthread_exit((void*) ret);
}

int main(int argc, char *argv[]) {
    FILE *file;
    size_t ret;

    double originalInnerProduct, globalProd = 0.0, *retProdThreads, variance;

    pthread_t *tid_sistema;

    if(argc < 3) { 
        printf("Use: <arquivo de entrada> <numero de threads> \n"); 
        exit(-1); 
    }

    file = fopen(argv[1], "rb");
    if(file == NULL) { 
        printf("--ERRO: fopen()\n"); 
        exit(-1); 
    }

    ret = fread(&n, sizeof(long int), 1, file);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }

    vector1 = malloc (sizeof(float) * n);
    vector2 = malloc (sizeof(float) * n);
    if(vector1 == NULL || vector2 == NULL) { 
        printf("--ERRO: malloc()\n"); 
        exit(-1); 
    }
    ret = fread(vector1, sizeof(float), n, file);
    if(ret < n) {
        fprintf(stderr, "Erro de leitura dos elementos do vetor 1\n");
        return 4;
    }
    ret = fread(vector2, sizeof(float), n, file);
    if(ret < n) {
        fprintf(stderr, "Erro de leitura dos elementos do vetor 2\n");
        return 4;
    }

    nthreads = atoi(argv[2]);
    if(nthreads > n) nthreads = n;

    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid_sistema == NULL) { 
        printf("--ERRO: malloc()\n"); 
        exit(-1); 
    }

    for(long int i = 0; i < nthreads; i++) {
        if (pthread_create(&tid_sistema[i], NULL, InnerProduct, (void*) i)) {
            printf("--ERRO: pthread_create()\n"); 
            exit(-1);
        }
    }

    for (int i = 0; i < nthreads; i++) {
        if (pthread_join(tid_sistema[i], (void *) &retProdThreads)) {
            printf("--ERRO: pthread_join()\n"); 
            exit(-1);
        }
        globalProd += *retProdThreads;
        free(retProdThreads);
    }

    printf("Produto Interno Concorrente  =  %.26lf\n", globalProd);
 
    ret = fread(&originalInnerProduct, sizeof(double), 1, file); 
    printf("\nProduto Interno Original  =  %.26lf\n", originalInnerProduct);

    variance = fabs((originalInnerProduct - globalProd) / originalInnerProduct);
    printf("\nA variancia entre o valor original e o encontrado, com concorrencia, eh: %.26lf \n", variance);

    free(vector1);
    free(vector2);
    free(tid_sistema);
    fclose(file);
    return 0;
}


