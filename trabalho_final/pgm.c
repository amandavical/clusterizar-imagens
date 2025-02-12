/* *************************************************************/
/* Alunos: Amanda Vieira, Derick Bessa,                        */
/* Rubens de Souza, Yolanda Marques                            */
/* Matriculas: 20241045050286, 20241045050332,                 */
/* 20241045050367, 20241045050405                              */
/* Avaliacao 04: Trabalho Final                                */
/* 04.505.23 - 2024.2 - Prof. Daniel Ferreira                  */
/* Compilador: gcc (Ubuntu 13.3.0) 13.3.0                      */
/* *************************************************************/

#include "pgm.h"
#include <stdio.h>
#include <stdlib.h>

// Função para ler uma imagem no formato PGM
void readPGMImage(struct pgm *pio, char *filename)
{
    FILE *fp; // Ponteiro para o arquivo
    char ch;  // Variável para ler caracteres do arquivo

    // Tenta abrir o arquivo no modo de leitura
    if (!(fp = fopen(filename, "r")))
    {
        perror("Erro."); // exibe mensagem de erro se não conseguir abrir
        exit(1);         // encerra o programa com código de erro 1
    }

    // Verifica se o arquivo começa com 'P', indicando que é um arquivo PGM
    // O formato PGM sempre começa com 'P' seguido de um número (2 ou 5).
    if ((ch = getc(fp)) != 'P')
    {
        puts("A imagem fornecida não está no formato pgm");
        exit(2);
    }

    // Lê o tipo do arquivo PGM (2 para texto, 5 para binário)
    // O tipo é um número que segue o 'P' no cabeçalho do arquivo.
    pio->tipo = getc(fp) - 48; // converte o caractere para número (ASCII '0' = 48)
    fseek(fp, 1, SEEK_CUR);    // avança uma posição no arquivo (pula o espaço após o tipo)

    // Ignora comentários no arquivo (linhas que começam com '#')
    // Comentários no PGM são opcionais e começam com '#'.
    while ((ch = getc(fp)) == '#')
    {
        while ((ch = getc(fp)) != '\n')
            ; // avança até o final da linha do comentário
    }

    // Volta uma posição no arquivo para ler as dimensões da imagem
    // é necessário porque o último caractere lido foi o '\n' do comentário.
    fseek(fp, -1, SEEK_CUR);

    // Lê as dimensões da imagem (colunas e linhas)
    // O formato PGM especifica as dimensões como dois números inteiros.
    fscanf(fp, "%d %d", &pio->c, &pio->r);
    if (ferror(fp))
    {                 // Verifica se houve erro na leitura
        perror(NULL); // Exibe mensagem de erro
        exit(3);      // Encerra o programa com código de erro 3
    }

    // Lê o valor máximo de intensidade dos pixels
    // Esse valor define o intervalo de intensidade dos pixels (geralmente 255).
    fscanf(fp, "%d", &pio->mv);
    fseek(fp, 1, SEEK_CUR); // Avança uma posição no arquivo (pula o espaço após o valor máximo)

    // Aloca memória para armazenar os dados da imagem
    // O tamanho da memória é calculado como (linhas * colunas * tamanho de um unsigned char).
    pio->pData = (unsigned char *)malloc(pio->r * pio->c * sizeof(unsigned char));

    // Verifica o tipo de arquivo PGM (2 para texto, 5 para binário)
    switch (pio->tipo)
    {
    case 2:
        puts("Lendo imagem PGM (dados em texto)");
        // Lê os dados da imagem em formato de texto
        // Cada pixel é lido como um número inteiro.
        for (int k = 0; k < (pio->r * pio->c); k++)
        {
            fscanf(fp, "%hhu", pio->pData + k); // Lê cada pixel e armazena no vetor pData
        }
        break;
    case 5:
        puts("Lendo imagem PGM (dados em binário)");
        // Lê os dados da imagem em formato binário
        // Os pixels são lidos diretamente como bytes.
        fread(pio->pData, sizeof(unsigned char), pio->r * pio->c, fp);
        break;
    default:
        puts("Não está implementado"); // Mensagem para tipos não suportados
    }

    fclose(fp); // fecha o arquivo após a leitura
}

// Função para escrever uma imagem no formato PGM
void writePGMImage(struct pgm *pio, char *filename)
{
    FILE *fp; // Ponteiro para o arquivo
    char ch;  // Variável para leitura de caracteres

    // Tenta abrir o arquivo no modo de escrita binária
    if (!(fp = fopen(filename, "wb")))
    {
        perror("Erro."); // Exibe mensagem de erro se não conseguir abrir
        exit(1);         // Encerra o programa com código de erro 1
    }

    // Escreve o cabeçalho do arquivo PGM
    // O cabeçalho define o tipo, as dimensões e o valor máximo de intensidade.
    fprintf(fp, "%s\n", "P5");              // Tipo PGM (binário)
    fprintf(fp, "%d %d\n", pio->c, pio->r); // dimensões da imagem (colunas x linhas)
    fprintf(fp, "%d\n", 255);               // Valor máximo de intensidade (fixo em 255)

    // Escreve os dados da imagem no arquivo
    // Os pixels são escritos diretamente como bytes.
    fwrite(pio->pData, sizeof(unsigned char), pio->c * pio->r, fp);

    fclose(fp); // Fecha o arquivo após a escrita
}

// Função para exibir informações e dados da imagem PGM
void viewPGMImage(struct pgm *pio)
{
    // Exibe informações básicas da imagem
    printf("Tipo: %d\n", pio->tipo);                // Tipo da imagem (2 ou 5)
    printf("Dimensões: [%d %d]\n", pio->c, pio->r); // Colunas x Linhas
    printf("Max: %d\n", pio->mv);                   // valor máximo de intensidade dos pixels

    // Exibe os dados da imagem (pixels)
    // Os pixels são exibidos em formato de matriz.
    for (int k = 0; k < (pio->r * pio->c); k++)
    {
        if (!(k % pio->c))
            printf("\n");                    // quebra a linha a cada 'c' pixels (formato de matriz)
        printf("%2hhu ", *(pio->pData + k)); // exibe o valor do pixel (formato de 2 dígitos)
    }
    printf("\n");
}