/* *************************************************************/
/* Alunos: Amanda Vieira, Derick Bessa,                        */
/* Rubens de Souza, Yolanda Marques                            */
/* Matriculas: 20241045050286, 20241045050332,                 */
/* 20241045050367, 20241045050405                              */
/* Avaliacao 04: Trabalho Final                                */
/* 04.505.23 - 2024.2 - Prof. Daniel Ferreira                  */
/* Compilador: gcc (Ubuntu 13.3.0) 13.3.0                      */
/* *************************************************************/

#include "kmeans.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

unsigned char *gerarVetor(unsigned tam) {
    return (unsigned char *)malloc(tam * sizeof(unsigned char));
}

unsigned *gerarVetorU(unsigned tam) {
    return (unsigned *)malloc(tam * sizeof(unsigned));
}

unsigned contarCoresDistintas(unsigned char *pCores, int tam) {
    unsigned cont = 0;
    for (int i = 0; i < tam; i++) {
        if (!IsCorRepetida(*(pCores + i), pCores, i))
            cont++;
    }
    return cont;
}

void preencherCoresDistintas(unsigned char *pCoresOrigem, int tam, unsigned char *pCoresDestino) {
    int j = 0;
    for (int i = 0; i < tam; i++) {
        if (!IsCorRepetida(*(pCoresOrigem + i), pCoresOrigem, i)) {
            *(pCoresDestino + j) = *(pCoresOrigem + i);
            j++;
        }
    }
}

void preencherVetorQtd(unsigned char *pOrig, int tamOrig, unsigned char *pComparacao, int tamComp, unsigned *pQtd) {
    for (int i = 0; i < tamComp; i++) {
        *(pQtd + i) = contarQtdCor(*(pComparacao + i), pOrig, tamOrig);
    }
}

void gerarCentroideInicial(unsigned char *centroides, int k, unsigned char *pCores, int qtdCores, int distMin) {
    *centroides = *pCores;
    int j = 1;
    for (int i = 1; i <= qtdCores; i++) {
        if (abs(*(centroides + j - 1) - *(pCores + i)) > distMin) {
            *(centroides + j) = *(pCores + i);
            j++;
        }
        if (j == k) break;
    }
}

void calcularGrupo(unsigned char *grupos, unsigned char *valores, int tamValores, unsigned char *centroides, int k) {
    for (int j = 0; j < tamValores; j++) {
        unsigned dist = abs(*(valores + j) - *centroides);
        unsigned char grupo = 0;
        for (int i = 1; i < k; i++) {
            if ((abs(*(valores + j) - *(centroides + i))) < dist) {
                dist = abs((*(valores + j) - *(centroides + i)));
                grupo = i;
            }
        }
        *(grupos + j) = grupo;
    }
}

void recalcularCentroide(unsigned char *centroides, int k, unsigned char *pCores, unsigned char *grupos, int qtdCores, int distMin) {
    unsigned char IsNovo = 1;
    int contExec = 0;

    while (IsNovo) {
        contExec++;
        IsNovo = 0;
        for (int i = 0; i < k; i++) {
            unsigned cont = 0;
            unsigned long media = 0;
            for (int j = 0; j < qtdCores; j++) {
                if (*(grupos + j) == i) {
                    media += *(pCores + j);
                    cont++;
                }
            }
            media = media / cont;

            if (distanciaMinima(media, centroides, k) > 1) {
                IsNovo = 1;
                *(centroides + i) = media;
            }
        }
        calcularGrupo(grupos, pCores, qtdCores, centroides, k);
    }
    printf("Quantidade de Cálculos do Centroide Final: %d\n\n", contExec);
}

void orgHistograma(unsigned char *pCores, unsigned int *pCont, int tam) {
    for (int i = 0; i < tam - 1; i++) {
        for (int j = 0; j < tam - 1; j++) {
            if (*(pCont + j) < *(pCont + j + 1)) {
                *(pCont + j) = *(pCont + j) ^ *(pCont + j + 1);
                *(pCont + j + 1) = *(pCont + j) ^ *(pCont + j + 1);
                *(pCont + j) = *(pCont + j) ^ *(pCont + j + 1);

                *(pCores + j) = *(pCores + j) ^ *(pCores + j + 1);
                *(pCores + j + 1) = *(pCores + j) ^ *(pCores + j + 1);
                *(pCores + j) = *(pCores + j) ^ *(pCores + j + 1);
            }
        }
    }
}

unsigned distanciaMinima(unsigned valor, unsigned char *centroides, int k) {
    unsigned distMin = abs(valor - *centroides);
    for (int i = 1; i < k; i++)
        distMin = (abs(valor - *(centroides + i)) < distMin) ? abs(valor - *(centroides + i)) : distMin;
    return distMin;
}

void simplificacao(unsigned char *pCores, unsigned char *grupos, int tam, unsigned char *centroides) {
    for (int i = 0; i < tam; i++) {
        *(pCores + i) = *(centroides + *(grupos + i));
    }
}