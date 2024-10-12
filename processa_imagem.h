#include <stdio.h>

#ifndef _processa_imagem_t_H
#define _processa_imagem_t_H

/* Define a estrutura da imagem PGM */
typedef struct imagemPGM {
    char formato[3]; /* p2 ou p5 */
    int largura; 
    int altura;
    int max_pix; /* tamanho da variação de cada pixel na escala de cinza */
    int **pixels; // pensar isso aqui direito // Aloca um ponteiro p ponteiro
} imagemPGM;

/* Le o formato, largura, altura e max_pix de um arquivo pgm 
   Considera que o arquivo não tem comentários
   Armazena as informações do cabeçalho na struct imagemPGM */
imagemPGM *le_cabecalho_pgm(FILE *arquivo, imagemPGM *imagem);

/* Le um arquivo P2 (ASCII) ou P5 (Binario)
   Armazena a matriz da imagem na struct imagemPGM */
imagemPGM *le_imagem(FILE *arquivo, imagemPGM *imagem);

/* Escreve um arquivo P2 (ASCII) ou P5 (Binario)
   Armazena a matriz da imagem na struct imagemPGM */
imagemPGM *escreve_imagem(FILE *arquivo, imagemPGM *imagem);

/* Calcula o LBP de todas as imagens de uma base
   Armazena o vetor lbp em um arquivo binario com o mesmo nome da img 
   e extensao .lbp */
void calcula_lbp(imagemPGM imagem);

/* Calcula a distancia Euclidiana entre duas imagens 
   considerando o histograma lpb delas */
int calcula_distancia(int hist_lbp1[], int hist_lbp2[]);

/* Gera uma imagem lbp na pasta 'imagens_lbp' 
   Nao possui nenhuma saida no terminal */
void gera_img_lbp (imagemPGM imagem);

/* Aloca memoria para a imagem */
int **aloca_imagem (imagemPGM *imagem);

/* Libera a memoria da imagem alocada */
int libera_memoria(int largura, int altura);

#endif
