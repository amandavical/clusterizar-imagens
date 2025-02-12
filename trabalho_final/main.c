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

int main(int argc, char *argv[])
{
    struct pgm img;         // estrutura que armazena os dados da imagem PGM
    clock_t time = clock(); // inicia a contagem do tempo de execução
    DIR *d;                 // ponteiro para o diretório
    struct dirent *dir;     // estrutura para armazenar informações dos arquivos no diretório
    d = opendir(DIR_ORIG);  // abre o diretório onde estão as imagens originais
    int contImg = 0;        // contador de imagens processadas

    if (d)
    { // verifica se o diretório foi aberto com sucesso
        while ((dir = readdir(d)) != NULL)
        { // lê cada arquivo no diretório
            if (contarCaracteres(dir->d_name) < 3)
                continue; // ignora arquivos com nomes muito curtos (como "." e "..")
            contImg++;    // incrementa o contador de imagens
            printf("----------------\n");
            printf("%s\n", dir->d_name); // imprime o nome do arquivo atual

            char path[100] = DIR_ORIG; // cria o caminho completo para a imagem
            strcat(path, dir->d_name); // concatena o nome do arquivo ao caminho

            readPGMImage(&img, path); // lê a imagem PGM e armazena na estrutura img

            int tamVet = img.c * img.r;                                  // calcula o tamanho do vetor de pixels (largura x altura)
            unsigned qtdCores = contarCoresDistintas(img.pData, tamVet); // conta quantas cores distintas existem na imagem

            unsigned char *pCoresDistintas = gerarVetor(qtdCores);       // cria um vetor para armazenar as cores distintas
            preencherCoresDistintas(img.pData, tamVet, pCoresDistintas); // preenche o vetor com as cores distintas encontradas

            unsigned *pContCores = gerarVetorU(qtdCores);                                // cria um vetor para contar a frequência de cada cor
            preencherVetorQtd(img.pData, tamVet, pCoresDistintas, qtdCores, pContCores); // preenche o vetor com a quantidade de cada cor
            orgHistograma(pCoresDistintas, pContCores, qtdCores);                        // organiza as cores em ordem decrescente de frequência

            printf("As 10 cores mais frequentes:\n");
            for (int i = 0; i < 10; i++)
            { // imprime as 10 cores mais frequentes
                printf("Cor: %hhu\tQtd: %u\n", *(pCoresDistintas + i), *(pContCores + i));
            }
            printf("\nTamanho do vetor = %d\n", tamVet);               // imprime o tamanho do vetor de pixels
            printf("Quantidade de cores distintas: %u\n\n", qtdCores); // imprime a quantidade de cores distintas

            free(pContCores); // libera a memória alocada para o vetor de contagem de cores

            unsigned char *centroides = gerarVetor(K);                                 // cria um vetor para armazenar os centroides do k-means
            gerarCentroideInicial(centroides, K, pCoresDistintas, qtdCores, DIST_MIN); // gera os centroides iniciais
            printf("Centroides Iniciais:\n");
            viewVetor(centroides, K); // imprime os centroides iniciais

            free(pCoresDistintas); // libera a memória alocada para o vetor de cores distintas

            unsigned char *grupos = gerarVetor(tamVet);                              // cria um vetor para armazenar os grupos de cada pixel
            calcularGrupo(grupos, img.pData, tamVet, centroides, K);                 // calcula o grupo de cada pixel com base nos centroides
            recalcularCentroide(centroides, K, img.pData, grupos, tamVet, DIST_MIN); // recalcula os centroides com base nos grupos
            simplificacao(img.pData, grupos, tamVet, centroides);                    // simplifica a imagem atribuindo a cada pixel a cor do seu centroide

            char nomeArq[100] = DIR_DEST; // cria o caminho para salvar a imagem processada
            strcat(nomeArq, "d-");        // adiciona um prefixo ao nome do arquivo
            strcat(nomeArq, dir->d_name); // concatena o nome original do arquivo
            writePGMImage(&img, nomeArq); // salva a imagem processada no diretório de destino

            printf("Centroides Finais:\n");
            viewVetor(centroides, K); // imprime os centroides finais

            free(grupos);     // libera a memória alocada para o vetor de grupos
            free(centroides); // libera a memória alocada para o vetor de centroides
        }
        closedir(d); // fecha o diretório
    }
    printf("Quantidade de imagens: %d\n", contImg); // imprime a quantidade total de imagens processadas

    time = clock() - time;                                                     // calcula o tempo total de execução
    printf("Tempo de execucao: %lf s\n", ((double)time) / ((CLOCKS_PER_SEC))); // imprime o tempo de execução em segundos

    return 0;
}