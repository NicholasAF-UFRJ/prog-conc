/* Aluno: Nicholas de Araujo Figueira DRE: 121088218 */

/* Arquivo para gerar dois vetores aleatórios */
/* A maior parte desse código foi baseado no código gera_vet_rand do lab2 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000 //valor maximo de um elemento do vetor
//#define TEXTO 

int main(int argc, char* argv[]) {
    int factor = 1; 
    long int n; 
    float *vector1, *vector2, elem; 
    double innerProduct = 0; 

    FILE * fileDescription; 
    size_t ret; 

    if(argc < 3) {
        fprintf(stderr, "Digite: <dimensao> <arquivo saida>\n");
        return 1;
    }
    n = atoi(argv[1]);

    //aloca memoria para o vetor 1 e 2
    vector1 = (float*) malloc(sizeof(float) * n);
    vector2 = (float*) malloc(sizeof(float) * n);
    if(!vector1 || !vector2) {
        fprintf(stderr, "Erro de alocao da memoria dos vetores\n");
        return 2;
    }

    srand(time(NULL));
    for(long int i = 0; i < n; i++) {
        vector1[i] = (rand() % MAX)/3.0 * factor;
        vector2[i] = (rand() % MAX)/3.0 * factor;
        innerProduct += vector1[i] * vector2[i];
        factor *= -1;
   }

    #ifdef TEXTO
    fprintf(stdout, "%ld\n", n);
    for(long int i=0; i<n; i++) {
        fprintf(stdout, "%f ",vector1[i]);
        fprintf(stdout, "%f ",vector2[i]);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "Produto Interno=%lf\n", innerProduct);
    #endif

    fileDescription = fopen(argv[2], "wb");
    if(!fileDescription) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 3;
    }


    ret = fwrite(&n, sizeof(long int), 1, fileDescription);
    ret = fwrite(vector1, sizeof(float), n, fileDescription);

    if(ret < n) {
      fprintf(stderr, "Erro de escrita no  arquivo\n");
      return 4;
    }
    ret = fwrite(vector2, sizeof(float), n, fileDescription);
    if(ret < n) {
      fprintf(stderr, "Erro de escrita no  arquivo\n");
      return 4;
    }

    ret = fwrite(&innerProduct, sizeof(double), 1, fileDescription);

    fclose(fileDescription);
    free(vector1);
    free(vector2);
    return 0;
}