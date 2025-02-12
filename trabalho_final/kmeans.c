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

// Função para alocar um vetor de caracteres não assinados
unsigned char *gerarVetor(unsigned tam)
{
    // Aloca memória para um vetor de 'tam' elementos do tipo unsigned char
    return (unsigned char *)malloc(tam * sizeof(unsigned char));
}

// Função para alocar um vetor de inteiros não assinados
unsigned *gerarVetorU(unsigned tam)
{
    // Aloca memória para um vetor de 'tam' elementos do tipo unsigned int
    return (unsigned *)malloc(tam * sizeof(unsigned));
}

// Função para contar o número de cores distintas em um vetor
unsigned contarCoresDistintas(unsigned char *pCores, int tam)
{
    unsigned cont = 0; // Contador de cores distintas

    // Percorre o vetor de cores
    for (int i = 0; i < tam; i++)
    {
        // Verifica se a cor atual já foi encontrada antes
        if (!IsCorRepetida(*(pCores + i), pCores, i))
            cont++; // Incrementa o contador se a cor for única
    }
    return cont; // Retorna o total de cores distintas
}

// Função para preencher um vetor com cores distintas
void preencherCoresDistintas(unsigned char *pCoresOrigem, int tam, unsigned char *pCoresDestino)
{
    int j = 0; // Índice para o vetor de destino

    // Percorre o vetor de origem
    for (int i = 0; i < tam; i++)
    {
        // Verifica se a cor atual é única
        if (!IsCorRepetida(*(pCoresOrigem + i), pCoresOrigem, i))
        {
            // Adiciona a cor ao vetor de destino
            *(pCoresDestino + j) = *(pCoresOrigem + i);
            j++; // Incrementa o índice do vetor de destino
        }
    }
}

// Função para preencher um vetor com a quantidade de cada cor
void preencherVetorQtd(unsigned char *pOrig, int tamOrig, unsigned char *pComparacao, int tamComp, unsigned *pQtd)
{
    // Percorre o vetor de cores distintas
    for (int i = 0; i < tamComp; i++)
    {
        // Conta quantas vezes a cor aparece no vetor original
        *(pQtd + i) = contarQtdCor(*(pComparacao + i), pOrig, tamOrig);
    }
}

// Função para gerar os centroides iniciais
void gerarCentroideInicial(unsigned char *centroides, int k, unsigned char *pCores, int qtdCores, int distMin)
{
    // O primeiro centroide recebe a primeira cor
    *centroides = *pCores;
    int j = 1; // Índice para os próximos centroides

    // Percorre o vetor de cores para definir os centroides iniciais
    for (int i = 1; i <= qtdCores; i++)
    {
        // Verifica se a cor atual está a uma distância mínima do último centroide
        if (abs(*(centroides + j - 1) - *(pCores + i)) > distMin)
        {
            // Adiciona a cor como um novo centroide
            *(centroides + j) = *(pCores + i);
            j++; // Incrementa o índice de centroides
        }
        // Para se já foram definidos 'k' centroides
        if (j == k)
            break;
    }
}

// Função para calcular o grupo de cada valor com base nos centroides
void calcularGrupo(unsigned char *grupos, unsigned char *valores, int tamValores, unsigned char *centroides, int k)
{
    // Percorre o vetor de valores
    for (int j = 0; j < tamValores; j++)
    {
        // Calcula a distância entre o valor atual e o primeiro centroide
        unsigned dist = abs(*(valores + j) - *centroides);
        unsigned char grupo = 0; // grupo inicial

        // Percorre os centroides para encontrar o mais próximo
        for (int i = 1; i < k; i++)
        {
            // Verifica se a distância para o centroide atual é menor
            if ((abs(*(valores + j) - *(centroides + i))) < dist)
            {
                dist = abs((*(valores + j) - *(centroides + i)));
                grupo = i; // atualiza o grupo
            }
        }
        // Define o grupo do valor atual
        *(grupos + j) = grupo;
    }
}

// Função para recalcular os centroides com base nos grupos atuais
void recalcularCentroide(unsigned char *centroides, int k, unsigned char *pCores, unsigned char *grupos, int qtdCores, int distMin)
{
    unsigned char IsNovo = 1; // Flag para verificar se os centroides mudaram
    int contExec = 0;         // Contador de execuções

    // Repete até que os centroides não mudem mais
    while (IsNovo)
    {
        contExec++;
        IsNovo = 0;

        // Percorre cada centroide
        for (int i = 0; i < k; i++)
        {
            unsigned cont = 0;       // Contador de valores no grupo
            unsigned long media = 0; // Soma dos valores no grupo

            // Percorre o vetor de cores para calcular a média do grupo
            for (int j = 0; j < qtdCores; j++)
            {
                if (*(grupos + j) == i)
                {
                    media += *(pCores + j);
                    cont++;
                }
            }
            // Calcula a média do grupo
            media = media / cont;

            // Verifica se a média está a uma distância mínima do centroide atual
            if (distanciaMinima(media, centroides, k) > 1)
            {
                IsNovo = 1;                // Indicando que o centroide mudou
                *(centroides + i) = media; // Atualiza o centroide
            }
        }
        // Recalcula os grupos com os novos centroides
        calcularGrupo(grupos, pCores, qtdCores, centroides, k);
    }
    // Exibe o número de execuções necessárias
    printf("Quantidade de Cálculos do Centroide Final: %d\n\n", contExec);
}

// Função para organizar o histograma em ordem decrescente
void orgHistograma(unsigned char *pCores, unsigned int *pCont, int tam)
{
    // Ordena o vetor de contagens e cores usando o método de troca (bubble sort)
    for (int i = 0; i < tam - 1; i++)
    {
        for (int j = 0; j < tam - 1; j++)
        {
            // Verifica se a contagem atual é menor que a próxima
            if (*(pCont + j) < *(pCont + j + 1))
            {
                // Troca as contagens
                *(pCont + j) = *(pCont + j) ^ *(pCont + j + 1);
                *(pCont + j + 1) = *(pCont + j) ^ *(pCont + j + 1);
                *(pCont + j) = *(pCont + j) ^ *(pCont + j + 1);

                // Troca as cores correspondentes
                *(pCores + j) = *(pCores + j) ^ *(pCores + j + 1);
                *(pCores + j + 1) = *(pCores + j) ^ *(pCores + j + 1);
                *(pCores + j) = *(pCores + j) ^ *(pCores + j + 1);
            }
        }
    }
}

// Função para calcular a distância mínima entre um valor e os centroides
unsigned distanciaMinima(unsigned valor, unsigned char *centroides, int k)
{
    // Inicializa a distância mínima com a distância para o primeiro centroide
    unsigned distMin = abs(valor - *centroides);

    // Percorre os centroides para encontrar a menor distância
    for (int i = 1; i < k; i++)
        distMin = (abs(valor - *(centroides + i)) < distMin) ? abs(valor - *(centroides + i)) : distMin;

    return distMin; // Retorna a distância mínima
}

// Função para simplificar a imagem, substituindo os valores pelos centroides
void simplificacao(unsigned char *pCores, unsigned char *grupos, int tam, unsigned char *centroides)
{
    // Percorre o vetor de cores
    for (int i = 0; i < tam; i++)
    {
        // Substitui o valor pelo centroide correspondente ao grupo
        *(pCores + i) = *(centroides + *(grupos + i));
    }
}