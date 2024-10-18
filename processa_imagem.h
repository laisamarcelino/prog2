#include <stdio.h>

#ifndef _processa_imagem_t_H
#define _processa_imagem_t_H

#define TAM 256
#define MAX_NOME 300

/* Define a estrutura da imagem PGM */
typedef struct imagemPGM {
    char *nome_arq; /* p2 ou p5 */
    int largura; 
    int altura;
    unsigned char *pixels; 
} imagemPGM;

/* Le a estrura de uma imagem PGM
   Retorna 1 em casos de erro, e 0 caso contrario */
int le_pgm(char caminho_img[], char *formato, int *largura, int *altura, 
        int *max_pixel, unsigned char **pixels);

/* Calcula o LBP de uma imagem*/
void calcula_lbp(imagemPGM *img_entrada, unsigned char **img_lbp);

/* Escreve o arquivo binario LBP */
void escreve_lbp(const char *caminho_lbp, unsigned char *img_lbp, int tam);

/* Le um arquivo LBP binario */
void le_lbp(const char *caminho_lbp, unsigned char **img_lbp, int tam_vet);

/* Calcula a distancia Euclidiana entre dois histogramas */
double calcula_dist(float *hist1, float *hist2, int tam_vet);

/* Processa as imagens do diretorio ./base
   Gera os arquivos lbp */
void manipula_dir(const char *nome_dir);

/* Compara uma imagem base com todas as imagens do diretorio */
void compara_imagens(char caminho_img[], const char *nome_dir);

/* Escreve uma imagem LBP */
void escreve_imagens(char caminho_img[], char caminho_saida[]);


#endif
