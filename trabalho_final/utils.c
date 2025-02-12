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

// funcao para verificar se uma cor já existe em um vetor de cores
unsigned char IsCorRepetida(unsigned char cor, unsigned char *pCores, int tam)
{
    unsigned char verif = 0; // Variável de verificação (0 = não repetida, 1 = repetida)

    // Percorre o vetor de cores
    for (int i = 0; i < tam; i++)
    {
        // Verifica se a cor atual do vetor é igual à cor fornecida
        if (*(pCores + i) == cor)
        {
            verif = 1; // Marca como repetida
            break;     // Sai do loop, pois a cor já foi encontrada
        }
    }

    return verif; // Retorna 1 se a cor for repetida, 0 caso contrário
}

// funcao para contar quantas vezes uma cor aparece em um vetor de cores
unsigned contarQtdCor(unsigned char cor, unsigned char *pCores, int tam)
{
    unsigned cont = 0; // Contador de ocorrências da cor

    // Percorre o vetor de cores
    for (int i = 0; i < tam; i++)
    {
        // Verifica se a cor atual do vetor é igual à cor fornecida
        if (*(pCores + i) == cor)
            cont++; // Incrementa o contador se a cor for encontrada
    }

    return cont; // Retorna o número de vezes que a cor aparece no vetor
}

// funcao para exibir os valores de um vetor de caracteres não assinados
void viewVetor(unsigned char *p, int tam)
{
    // Percorre o vetor e exibe cada valor
    for (int i = 0; i < tam; i++)
    {
        printf("%hhu\n", *(p + i)); // Exibe o valor do vetor na posição i
    }
    printf("\n"); // Adiciona uma linha em branco no final para melhorar a legibilidade
}

// funcao para contar o número de caracteres em uma string
unsigned contarCaracteres(char *p)
{
    unsigned cont = 0; // Contador de caracteres

    // Percorre a string até encontrar o caractere nulo ('\0')
    while (*(p + cont) != '\0')
        cont++; // incrementa o contador para cada caractere válido

    return cont; // Retorna o número de caracteres na string
}