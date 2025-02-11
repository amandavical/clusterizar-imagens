/* *************************************************************/
/* Alunos: Amanda Vieira, Derick Bessa,                        */
/* Rubens de Souza, Yolanda Marques                            */
/* Matriculas: 20241045050286, 20241045050332,                 */
/* 20241045050367, 20241045050405                              */
/* Avaliacao 04: Trabalho Final                                */
/* 04.505.23 - 2024.2 - Prof. Daniel Ferreira                  */
/* Compilador: gcc (Ubuntu 13.3.0) 13.3.0                      */
/* *************************************************************/

#include "utils.h"
#include <stdio.h>

unsigned char IsCorRepetida(unsigned char cor, unsigned char *pCores, int tam) {
    unsigned char verif = 0;
    for (int i = 0; i < tam; i++) {
        if (*(pCores + i) == cor) {
            verif = 1;
            break;
        }
    }
    return verif;
}

unsigned contarQtdCor(unsigned char cor, unsigned char *pCores, int tam) {
    unsigned cont = 0;
    for (int i = 0; i < tam; i++) {
        if (*(pCores + i) == cor)
            cont++;
    }
    return cont;
}

void viewVetor(unsigned char *p, int tam) {
    for (int i = 0; i < tam; i++) {
        printf("%hhu\n", *(p + i));
    }
    printf("\n");
}

unsigned contarCaracteres(char *p) {
    unsigned cont = 0;
    while (*(p + cont) != '\0')
        cont++;
    return cont;
}