/* *************************************************************/
/* Alunos: Amanda Vieira, Derick Bessa,                        */
/* Rubens de Souza, Yolanda Marques                            */
/* Matriculas: 20241045050286, 20241045050332,                 */
/* 20241045050367, 20241045050405                              */
/* Avaliacao 04: Trabalho Final                                */
/* 04.505.23 - 2024.2 - Prof. Daniel Ferreira                  */
/* Compilador: gcc (Ubuntu 13.3.0) 13.3.0                      */
/* *************************************************************/

#ifndef KMEANS_H
#define KMEANS_H

unsigned char *gerarVetor(unsigned);
unsigned *gerarVetorU(unsigned);
unsigned contarCoresDistintas(unsigned char *, int);
void preencherCoresDistintas(unsigned char *, int, unsigned char *);
void preencherVetorQtd(unsigned char *, int, unsigned char *, int, unsigned *);
void gerarCentroideInicial(unsigned char *, int, unsigned char *, int, int);
void calcularGrupo(unsigned char *, unsigned char *, int, unsigned char *, int);
void recalcularCentroide(unsigned char *, int, unsigned char *, unsigned char *, int, int);
void orgHistograma(unsigned char *, unsigned int *, int);
unsigned distanciaMinima(unsigned, unsigned char *, int);
void simplificacao(unsigned char *, unsigned char *, int, unsigned char *);

#endif

