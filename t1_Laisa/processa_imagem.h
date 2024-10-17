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

void le_pgm(const char *nome_arq, int *largura, int *altura, unsigned char **pixels);
void compara_imagens(const char *nome_arq);


#endif
