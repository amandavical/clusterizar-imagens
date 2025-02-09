#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <string.h>

#define K 5

// Diretórios das imagens
#define DIR_ORIG "./images/"
#define DIR_DEST "./cluster-images/"

// Distancia mínima dos centroides iniciais
#define DIST_MIN 4

struct pgm{
	int tipo;
	int c;
	int r;
	int mv;
	unsigned char *pData;
};

void readPGMImage(struct pgm *, char *);
void viewPGMImage(struct pgm *);
void writePGMImage(struct pgm *, char *);
unsigned char IsCorRepetida(unsigned char, unsigned char *, int);
unsigned char *gerarVetor(unsigned);
unsigned *gerarVetorU(unsigned);
unsigned contarCoresDistintas(unsigned char *, int);
void viewVetor(unsigned char *, int);
void preencherCoresDistintas(unsigned char *, int, unsigned char *);
unsigned contarQtdCor(unsigned char, unsigned char *, int);
void preencherVetorQtd(unsigned char *, int, unsigned char *, int, unsigned *);
void gerarCentroideInicial(unsigned char *, int, unsigned char *, int, int);
void calcularGrupo(unsigned char *, unsigned char *, int, unsigned char *, int);
void recalcularCentroide(unsigned char *, int, unsigned char *, unsigned char *, int, int);
void orgHistograma(unsigned char *, unsigned int *, int);
unsigned distanciaMinima(unsigned, unsigned char *, int);
void simplificacao(unsigned char *, unsigned char *, int, unsigned char *);
void ordVetAscend(unsigned char *, int);
unsigned contarCaracteres(char *p);

int main(int argc, char *argv[]){

	struct pgm img;

	// Variável para o cálculo do tempo de execução
	clock_t time = clock();

    DIR *d;
    struct dirent *dir;
    d = opendir(DIR_ORIG);
    int contImg = 0;
    if(d){   
        while((dir = readdir(d)) != NULL){
            if(contarCaracteres(dir->d_name) < 3) continue;
            contImg++;
            printf("----------------\n");
            printf("%s\n", dir->d_name);

            char path[100] = DIR_ORIG;
            strcat(path, dir->d_name);

            readPGMImage(&img, path);
            //viewPGMImage(&img);

            int tamVet = img.c*img.r;

            unsigned qtdCores = contarCoresDistintas(img.pData, tamVet);
            
            // Gerar vetor das cores distintas
            unsigned char *pCoresDistintas = NULL;
            if(!(pCoresDistintas = gerarVetor(qtdCores))){
                printf("Não há memória suficiente\n\n");
                exit(1);
            }
            preencherCoresDistintas(img.pData, tamVet, pCoresDistintas);
            
            // Vetor da quantidade de cada cor da imagem
            unsigned *pContCores = NULL;
            if(!(pContCores = gerarVetorU(qtdCores))){
                printf("Não há memória suficiente\n\n");
                exit(1);
            }

            // Preenche vetor da quantidade das cores e organiza pelas mais frequentes
            preencherVetorQtd(img.pData, tamVet, pCoresDistintas, qtdCores, pContCores);
            orgHistograma(pCoresDistintas, pContCores, qtdCores);

            // Prints para Debug
            printf("As 10 cores mais frequentes:\n");
            for(int i=0; i<10; i++){
                printf("Cor: %hhu\tQtd: %u\n", *(pCoresDistintas+i), *(pContCores+i));
            }
            printf("\n");
            printf("Tamanho do vetor = %d\n", tamVet);
            printf("Quantidade de cores distintas: %u\n\n", qtdCores);
            // ..

            //Vetor da quantidade serviu apenas para organizar o histograma
            free(pContCores);


            // Criação dos centroides iniciais
            unsigned char *centroides = NULL;
            if(!(centroides = gerarVetor(K))){
                printf("Não há memória suficiente\n\n");
                exit(1);
            }

            //Define os centroides iniciais baseados no histograma respeitando a distância mínima
            gerarCentroideInicial(centroides, K, pCoresDistintas, qtdCores, DIST_MIN);
            printf("Centroides Iniciais:\n");
            viewVetor(centroides, K);
            // ..

            // Só precisa das cores distintas para calcular os primeiros centroides
            free(pCoresDistintas);

            // Vetor da definição dos grupos das cores da imagem
            unsigned char *grupos = NULL;
            if(!(grupos = gerarVetor(tamVet))){
                printf("Não há memória suficiente\n\n");
                exit(1);
            }

            // Classificação das cores da imagem em grupos baseados nos centroides
            calcularGrupo(grupos, img.pData, tamVet, centroides, K);
            
            recalcularCentroide(centroides, K, img.pData, grupos, tamVet, DIST_MIN);
            
            simplificacao(img.pData, grupos, tamVet, centroides);

            // Criar imagem simplificada
            char nomeArq[100] = DIR_DEST;
            strcat(nomeArq, "d-");
            strcat(nomeArq, dir->d_name);
            writePGMImage(&img, nomeArq);
            
            
            // Prints para debug
            printf("Centroides Finais:\n");
            viewVetor(centroides, K);
            // ..

            // Liberação de Memória
            free(grupos);
            free(centroides);
        }
        closedir(d);
    }
    printf("Quantidade de imagens: %d\n", contImg);

    time = clock() - time;
    // imprime o tempo na tela
    // CLOCKS_PER_SEC/1000 para milissegundos. Se quer em segundos, só precisa dividir por CLOCKS_PER_SEC
    printf("Tempo de execucao: %lf ms\n", ((double)time)/((CLOCKS_PER_SEC)));

    return 0;

}

void readPGMImage(struct pgm *pio, char *filename){

	FILE *fp;
	char ch;

	if (!(fp = fopen(filename,"r"))){
		perror("Erro.");
		exit(1);
	}

	if ( (ch = getc(fp))!='P'){
		puts("A imagem fornecida não está no formato pgm");
		exit(2);
	}
	
	pio->tipo = getc(fp)-48;
	
	fseek(fp,1, SEEK_CUR);

	while((ch=getc(fp))=='#'){
		while( (ch=getc(fp))!='\n');
	}

	fseek(fp,-1, SEEK_CUR);

	fscanf(fp, "%d %d",&pio->c,&pio->r);
	if (ferror(fp)){ 
		perror(NULL);
		exit(3);
	}	
	fscanf(fp, "%d",&pio->mv);
	fseek(fp,1, SEEK_CUR);

	pio->pData = (unsigned char*) malloc(pio->r * pio->c * sizeof(unsigned char));

	switch(pio->tipo){
		case 2:
			puts("Lendo imagem PGM (dados em texto)");
			for (int k=0; k < (pio->r * pio->c); k++){
				fscanf(fp, "%hhu", pio->pData+k);
			}
		break;	
		case 5:
			puts("Lendo imagem PGM (dados em binário)");
			fread(pio->pData,sizeof(unsigned char),pio->r * pio->c, fp);
		break;
		default:
			puts("Não está implementado");
	}
	
	fclose(fp);

}

void writePGMImage(struct pgm *pio, char *filename){
	FILE *fp;
	char ch;

	if (!(fp = fopen(filename,"wb"))){
		perror("Erro.");
		exit(1);
	}

	fprintf(fp, "%s\n","P5");
	fprintf(fp, "%d %d\n",pio->c, pio->r);
	fprintf(fp, "%d\n", 255);

	fwrite(pio->pData, sizeof(unsigned char),pio->c * pio->r, fp);

	fclose(fp);

}


void viewPGMImage(struct pgm *pio){
	printf("Tipo: %d\n",pio->tipo);
	printf("Dimensões: [%d %d]\n",pio->c, pio->r);
	printf("Max: %d\n",pio->mv);

	for (int k=0; k < (pio->r * pio->c); k++){
		if (!( k % pio->c)) printf("\n");
		printf("%2hhu ",*(pio->pData+k));
	}	
	printf("\n");
}

unsigned char IsCorRepetida(unsigned char cor, unsigned char *pCores, int tam){
	unsigned char verif = 0;
	for(int i=0; i<tam; i++){
		if(*(pCores+i) == cor){
			verif = 1;
			break;
		}
	}
	return verif;
}

unsigned char *gerarVetor(unsigned tam){
	unsigned char *p = NULL;
	p = (unsigned char *) malloc(tam * sizeof(unsigned char));
	return p;
}

unsigned *gerarVetorU(unsigned tam){
	unsigned *p = NULL;
	p = (unsigned *) malloc(tam * sizeof(unsigned));
	return p;
}

unsigned contarCoresDistintas(unsigned char *pCores, int tam){
	unsigned cont = 0;
	for(int i=0; i<tam; i++){
		if(!(IsCorRepetida(*(pCores+i), pCores, i)))
			cont++;
	}

	return cont;
}

void viewVetor(unsigned char *p, int tam){
	for(int i=0; i<tam; i++){
		printf("%hhu\n", *(p+i));
	}
	printf("\n");
}

void preencherCoresDistintas(unsigned char *pCoresOrigem, int tam, unsigned char *pCoresDestino){
	int j = 0;
	for(int i=0; i<tam; i++){
		if(!(IsCorRepetida(*(pCoresOrigem+i), pCoresOrigem, i))){
			*(pCoresDestino+j) = *(pCoresOrigem+i);
			j++;
		}
	}
}

unsigned contarQtdCor(unsigned char cor, unsigned char *pCores, int tam){
	unsigned cont = 0;
	for(int i=0; i<tam; i++){
		if(*(pCores+i) == cor)
			cont++;
	}
	return cont;
}

void preencherVetorQtd(unsigned char *pOrig, int tamOrig, unsigned char *pComparacao, int tamComp, unsigned *pQtd){
	for(int i=0; i<tamComp; i++){
		*(pQtd+i) = contarQtdCor(*(pComparacao+i), pOrig, tamOrig);
	}
}

void gerarCentroideInicial(unsigned char *centroides, int k, unsigned char *pCores, int qtdCores, int distMin){
	// primeiro centroide recebe a primeira cor
	*centroides = *pCores;
	int j = 1;
	for(int i=1; i<=qtdCores; i++){
		if(abs(*(centroides + j - 1) - *(pCores+i)) > distMin){
			*(centroides+j) = *(pCores+i);
			j++;
		}
		if(j == k) break;
	}
}

void calcularGrupo(unsigned char *grupos, unsigned char *valores, int tamValores, unsigned char *centroides, int k){
	
	for(int j=0; j<tamValores; j++){
		unsigned dist = abs(*(valores+j) - *centroides);
		unsigned char grupo = 0;
		for(int i=1; i<k; i++){
			if((abs(*(valores+j) - *(centroides+i))) < dist){
				dist = abs((*(valores+j) - *(centroides+i)));
				grupo = i;
			}
		}
		*(grupos+j) = grupo;
	}
}

void recalcularCentroide(unsigned char *centroides, int k, unsigned char *pCores, unsigned char *grupos, int qtdCores, int distMin){
	unsigned char IsNovo = 1;
	int contExec = 0;

	// O Código repete até que o centroide esteja dentro da distancia mínima da média
	while(IsNovo){
		contExec++;
		IsNovo = 0;	
		for(int i=0; i<k; i++){
			unsigned cont = 0;
			unsigned long media = 0;
			for(int j=0;j<qtdCores; j++){
				if(*(grupos+j) == i){
					media += *(pCores+j);
					cont++;
				}
			}
			media = media / cont;

			// Aqui seria uma média "exata", tire o comentário e comente a linha de baixo para testar
            
			//if(*(centroides+i) != media){				// mais cálculos para a média "exata"

			// Se a diferença da média pro centroide for de apenas 1 pixel, ele não muda. 
			// Pode ser que economize um pouco de tempo, mas se quiser a média "exata" pode 
			// comentar a linha de baixo e tirar o comentário da linha de cima
			
			if(distanciaMinima(media, centroides, k) > 1){   // pode economizar um pouco de tempo
				IsNovo = 1;
				*(centroides+i) = media;
			}
		}
		calcularGrupo(grupos, pCores, qtdCores, centroides, k);
	}
	printf("Quantidade de Cálculos do Centroide Final: %d\n\n", contExec);
}

// Organiza o vetor da maior contagem para a menor
void orgHistograma(unsigned char *pCores, unsigned int *pCont, int tam){
	for(int i=0; i<tam-1; i++){
        for(int j=0; j<tam-1;j++){
            if(*(pCont+j) < *(pCont+j+1)){
                // Organizar vetor contagem
				*(pCont+j) = *(pCont+j) ^ *(pCont+j+1);
                *(pCont+j+1) = *(pCont+j) ^ *(pCont+j+1);
                *(pCont+j) = *(pCont+j) ^ *(pCont+j+1);

				// Organizar vetor das cores distintas
				*(pCores+j) = *(pCores+j) ^ *(pCores+j+1);
                *(pCores+j+1) = *(pCores+j) ^ *(pCores+j+1);
                *(pCores+j) = *(pCores+j) ^ *(pCores+j+1);
            }
        }
    }
}

unsigned distanciaMinima(unsigned valor, unsigned char *centroides, int k){
	// Distância do primeiro centroide
	unsigned distMin = abs(valor - *centroides);
	
	for(int i=1; i<k; i++)
		distMin = (abs(valor - *(centroides + i)) < distMin)? abs(valor - *(centroides + i)): distMin;

	return distMin;
}

// Processo antes de gerar a imagem nova, onde troca os pixels antigos
void simplificacao(unsigned char *pCores, unsigned char *grupos, int tam, unsigned char *centroides){
	for(int i=0; i<tam; i++){
		*(pCores+i) = *(centroides + *(grupos+i));
	}
}

void ordVetAscend(unsigned char *p, int tam){
	for(int i = 0; i < tam - 1; i++){
        for(int j = i + 1; j < tam; j++){
            if(*(p+j) < *(p+i)){
                *(p+j) = *(p+j) ^ *(p+i);
                *(p+i) = *(p+j) ^ *(p+i);
                *(p+j) = *(p+j) ^ *(p+i);
            }
        }

    }
}

// Usei essa função abaixo para ignorar o "." e ".." que o diretório lia, para pegar apenas os arquivos 
// .pgm comparando com um valor mínimo de carateres
unsigned contarCaracteres(char *p){
    unsigned cont = 0;
    while(*(p+cont) != '\0')
        cont++;
    return cont;
}