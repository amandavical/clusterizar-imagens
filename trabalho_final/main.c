/* *************************************************************/
/* Alunos: Amanda Vieira, Derick Bessa,                        */
/* Rubens de Souza, Yolanda Marques                            */
/* Matriculas: 20241045050286, 20241045050332,                 */
/* 20241045050367, 20241045050405                              */
/* Avaliacao 04: Trabalho Final                                */
/* 04.505.23 - 2024.2 - Prof. Daniel Ferreira                  */
/* Compilador: gcc (Ubuntu 13.3.0) 13.3.0                      */
/* *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include "pgm.h"
#include "kmeans.h"
#include "utils.h"

#define K 5
#define DIR_ORIG "./images/"
#define DIR_DEST "./cluster-images/"
#define DIST_MIN 4

int main(int argc, char *argv[]) {
    struct pgm img;
    clock_t time = clock();
    DIR *d;
    struct dirent *dir;
    d = opendir(DIR_ORIG);
    int contImg = 0;

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (contarCaracteres(dir->d_name) < 3) continue;
            contImg++;
            printf("----------------\n");
            printf("%s\n", dir->d_name);

            char path[100] = DIR_ORIG;
            strcat(path, dir->d_name);

            readPGMImage(&img, path);

            int tamVet = img.c * img.r;
            unsigned qtdCores = contarCoresDistintas(img.pData, tamVet);

            unsigned char *pCoresDistintas = gerarVetor(qtdCores);
            preencherCoresDistintas(img.pData, tamVet, pCoresDistintas);

            unsigned *pContCores = gerarVetorU(qtdCores);
            preencherVetorQtd(img.pData, tamVet, pCoresDistintas, qtdCores, pContCores);
            orgHistograma(pCoresDistintas, pContCores, qtdCores);

            printf("As 10 cores mais frequentes:\n");
            for (int i = 0; i < 10; i++) {
                printf("Cor: %hhu\tQtd: %u\n", *(pCoresDistintas + i), *(pContCores + i));
            }
            printf("\nTamanho do vetor = %d\n", tamVet);
            printf("Quantidade de cores distintas: %u\n\n", qtdCores);

            free(pContCores);

            unsigned char *centroides = gerarVetor(K);
            gerarCentroideInicial(centroides, K, pCoresDistintas, qtdCores, DIST_MIN);
            printf("Centroides Iniciais:\n");
            viewVetor(centroides, K);

            free(pCoresDistintas);

            unsigned char *grupos = gerarVetor(tamVet);
            calcularGrupo(grupos, img.pData, tamVet, centroides, K);
            recalcularCentroide(centroides, K, img.pData, grupos, tamVet, DIST_MIN);
            simplificacao(img.pData, grupos, tamVet, centroides);

            char nomeArq[100] = DIR_DEST;
            strcat(nomeArq, "d-");
            strcat(nomeArq, dir->d_name);
            writePGMImage(&img, nomeArq);

            printf("Centroides Finais:\n");
            viewVetor(centroides, K);

            free(grupos);
            free(centroides);
        }
        closedir(d);
    }
    printf("Quantidade de imagens: %d\n", contImg);

    time = clock() - time;
    printf("Tempo de execucao: %lf ms\n", ((double)time) / ((CLOCKS_PER_SEC)));

    return 0;
}