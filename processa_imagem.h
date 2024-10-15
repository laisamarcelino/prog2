#include <stdio.h>

#ifndef _processa_imagem_t_H
#define _processa_imagem_t_H

/* Define a estrutura da imagem PGM */
typedef struct imagemPGM {
    char formato[3]; /* p2 ou p5 */
    int largura; 
    int altura;
    int max_pix; /* tamanho da variação de cada pixel na escala de cinza */
    int **pixels; 
} imagemPGM;

/* Le o formato, largura, altura e max_pix de um arquivo pgm 
   Considera que o arquivo não tem comentários
   Armazena as informações do cabeçalho na struct imagemPGM */
imagemPGM *le_cabecalho_imagem(FILE *arquivo, imagemPGM *imagem);

/* Le um arquivo P2 (ASCII) ou P5 (Binario)
   Armazena a matriz da imagem na struct imagemPGM */
imagemPGM *le_imagem(FILE *arquivo, imagemPGM *imagem);

/* Escreve um arquivo P2 (ASCII) ou P5 (Binario)
   Escreve uma imagem pgm apos o calculo do lbp */
imagemPGM *escreve_imagem_lbp(FILE *arquivo, imagemPGM *imagem);

/* Calcula o LBP de uma imagem da base
   Gera a matriz da nova imagem apos calculo do lbp */
void calcula_lbp(imagemPGM *imagem, imagemPGM *lbp_imagem);

/* Gera um histograma que sumariza a ocorrencia de cada um
   dos 256 valores lbp. Armazena o vetor em um arquivo
   binario com o mesmo nome da imagem e extensao .lbp 
   Retorna o nome do aquivo .lbp */
void gera_histograma(imagemPGM *lbp_imagem, int *histograma, char *nome_pgm);

/* Le um arquivo binario lbp e retorna o tamanho do histograma */
int le_histograma_lbp (int *histograma, char *nome_lbp);

/* Calcula a distancia Euclidiana entre duas imagens 
   considerando o histograma lpb delas */
int calcula_distancia(int *histograma1, int *histograma2, int tam);

/* Utiliza o histograma das imagens para comparar duas imagens
   a partir do calculo da distancia entre dois vetores 
   - distancia Euclediana */
void compara_imagens (int *histograma1, int *histograma2);

/* Retorna 1 se for uma imagem valida, de extensao .pgm
   ou 0 caso contrario */
int imagem_valida(char *nome_pgm);

/* Aloca memória para a estrutura da imagem LBP */
void aloca_lbp (imagemPGM *imagem, imagemPGM *lbp_imagem);

/* Aloca memoria para a matriz da imagem */
int **aloca_matriz_imagem (imagemPGM *imagem);

/* Libera a memoria da imagem alocada */
void libera_memoria(imagemPGM *imagem);

#endif
